import os
os.environ["QT_QPA_PLATFORM"] = "xcb"  # Forzar backend X11 (evita bug de Wayland)

import cv2
import numpy as np
import serial
import time

# Variables globales para recordar el último envío al arduino
_last_signal = None
_last_send_time = 0


# ────────────────────────────────────────────────────────────────
# 1️⃣ Inicialización de cámara e interfaz
# ────────────────────────────────────────────────────────────────

def initialize_camera(index: int = 1): #Por default 0 es la camara integrada, 1 una camara usb externa.
    """Inicializa la cámara y devuelve el objeto VideoCapture."""
    cap = cv2.VideoCapture(index)
    if not cap.isOpened():
        raise IOError("❌ No se pudo acceder a la cámara.")
    return cap


def create_window(window_name: str, width: int = 640, height: int = 480):
    """Crea una ventana única y ajustable para la visualización."""
    cv2.namedWindow(window_name, cv2.WINDOW_NORMAL)
    cv2.resizeWindow(window_name, width, height)


# ────────────────────────────────────────────────────────────────
# 2️⃣ Procesamiento y detección de colores
# ────────────────────────────────────────────────────────────────

def detect_colors(frame: np.ndarray):
    """
    Detecta los objetos de color rojo, verde y azul en un frame.
    Devuelve una lista de detecciones con (color, x, y, w, h, cx, cy).
    """
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # Rangos de color HSV
    lower_red1, upper_red1 = np.array([0, 120, 70]), np.array([10, 255, 255])
    lower_red2, upper_red2 = np.array([170, 120, 70]), np.array([180, 255, 255])
    lower_green, upper_green = np.array([36, 50, 70]), np.array([89, 255, 255])
    lower_blue, upper_blue = np.array([90, 60, 70]), np.array([128, 255, 255])

    # Máscaras binarias
    mask_red = cv2.inRange(hsv, lower_red1, upper_red1) + cv2.inRange(hsv, lower_red2, upper_red2)
    mask_green = cv2.inRange(hsv, lower_green, upper_green)
    mask_blue = cv2.inRange(hsv, lower_blue, upper_blue)

    masks = {
        "Rojo": (mask_red, (0, 0, 255)),
        "Verde": (mask_green, (0, 255, 0)),
        "Azul": (mask_blue, (255, 0, 0))
    }

    detections = []

    for color_name, (mask, bgr_color) in masks.items():
        contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        for cnt in contours:
            if cv2.contourArea(cnt) < 1000:
                continue

            x, y, w, h = cv2.boundingRect(cnt)
            cx, cy = x + w // 2, y + h // 2
            detections.append({
                "color": color_name,
                "bgr": bgr_color,
                "x": x, "y": y, "w": w, "h": h,
                "cx": cx, "cy": cy
            })

    return detections


# ────────────────────────────────────────────────────────────────
# 3️⃣ Determinación de posición / cuadrante
# ────────────────────────────────────────────────────────────────

def determine_position(cx: int, frame_width: int):
    """
    Determina en qué cuadrante horizontal está el objeto:
    IZQUIERDA, CENTRO o DERECHA.
    """
    region_left = frame_width // 3
    region_right = 2 * frame_width // 3

    if cx < region_left:
        return "IZQUIERDA"
    elif cx > region_right:
        return "DERECHA"
    else:
        return "CENTRO"


# ────────────────────────────────────────────────────────────────
# 4️⃣ Vector de posición del color rojo
# ────────────────────────────────────────────────────────────────

def get_red_position_vector(detections: list, frame_width: int):
    """
    Devuelve un vector según la posición del color rojo:
    [1,0,0] -> izquierda
    [0,1,0] -> centro
    [0,0,1] -> derecha
    [0,0,0] -> no detectado
    """
    for det in detections:
        if det["color"] == "Rojo":
            pos = determine_position(det["cx"], frame_width)
            if pos == "IZQUIERDA":
                return [1, 0, 0]
            elif pos == "CENTRO":
                return [0, 1, 0]
            elif pos == "DERECHA":
                return [0, 0, 1]
    return [0, 0, 0]


# ────────────────────────────────────────────────────────────────
# 5️⃣ Comunicación Serial
# ────────────────────────────────────────────────────────────────

def initialize_serial(port: str = "/dev/ttyACM0", baudrate: int = 9600):
    """Inicializa la comunicación serial con Arduino."""
    try:
        arduino = serial.Serial(port, baudrate, timeout=1)
        time.sleep(2)  # Espera a que el Arduino se reinicie
        print(f"✅ Conectado a Arduino en {port}")
        return arduino
    except serial.SerialException as e:
        print(f"⚠️ No se pudo abrir el puerto serial: {e}")
        return None


def send_serial_signal(arduino, red_vector: list, send_interval: float = 0.5):
    """
    Envía señal al Arduino a una frecuencia máxima fija (send_interval)
    solo si la información cambió desde el último envío.

    red_vector:
        [1,0,0] -> 'L' (izquierda)
        [0,1,0] -> 'C' (centro)
        [0,0,1] -> 'R' (derecha)
        [0,0,0] -> 'N' (no detectado)
    """
    global _last_signal, _last_send_time

    if arduino is None:
        return

    # Traducir el vector a un carácter
    if red_vector == [1, 0, 0]:
        signal = 'L'
    elif red_vector == [0, 1, 0]:
        signal = 'C'
    elif red_vector == [0, 0, 1]:
        signal = 'R'
    else:
        signal = 'N'

    now = time.time()
    elapsed = now - _last_send_time

    # Enviar solo si cambió la señal o pasó más tiempo del permitido
    if signal != _last_signal:
        try:
            arduino.write(f"{signal}\n".encode('utf-8'))
            _last_signal = signal
            _last_send_time = now
            print(f"→ Señal enviada: {signal} (Δt={elapsed:.2f}s)")
        except serial.SerialException:
            print("⚠️ Error al enviar señal serial.")


# ────────────────────────────────────────────────────────────────
# 6️⃣ Dibujo y visualización
# ────────────────────────────────────────────────────────────────

def draw_detections(frame: np.ndarray, detections: list):
    """Dibuja rectángulos, texto y líneas divisorias en el frame."""
    height, width = frame.shape[:2]
    region_left = width // 3
    region_right = 2 * width // 3

    # Dibujar líneas divisorias
    cv2.line(frame, (region_left, 0), (region_left, height), (255, 255, 255), 2)
    cv2.line(frame, (region_right, 0), (region_right, height), (255, 255, 255), 2)

    for det in detections:
        pos = determine_position(det["cx"], width)
        x, y, w, h = det["x"], det["y"], det["w"], det["h"]
        color_bgr = det["bgr"]

        cv2.rectangle(frame, (x, y), (x + w, y + h), color_bgr, 2)
        cv2.circle(frame, (det["cx"], det["cy"]), 5, color_bgr, -1)
        cv2.putText(frame, f"{det['color']}: {pos}", (x, y - 10),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.6, color_bgr, 2)

    return frame


# ────────────────────────────────────────────────────────────────
# 7️⃣ Interfaz principal
# ────────────────────────────────────────────────────────────────

def run_interface():
    """Ejecuta el flujo principal de captura, detección y comunicación serial."""
    window_name = "Detección de colores"
    cap = initialize_camera(0)
    create_window(window_name)
    arduino = initialize_serial("/dev/ttyACM0", 9600)

    print("✅ Cámara inicializada. Presiona ESC o cierra la ventana para salir.")

    while cv2.getWindowProperty(window_name, cv2.WND_PROP_VISIBLE) >= 1:
        ret, frame = cap.read()
        if not ret:
            break

        detections = detect_colors(frame)
        frame = draw_detections(frame, detections)

        # Calcular vector y enviar por serial
        height, width = frame.shape[:2]
        red_vector = get_red_position_vector(detections, width)
        send_serial_signal(arduino, red_vector)

        cv2.imshow(window_name, frame)

        if cv2.waitKey(1) & 0xFF == 27:
            break

    cap.release()
    cv2.destroyAllWindows()
    if arduino:
        arduino.close()
        print("🔌 Conexión serial cerrada.")


# ────────────────────────────────────────────────────────────────
# 8️⃣ Punto de entrada
# ────────────────────────────────────────────────────────────────

if __name__ == "__main__":
    run_interface()
