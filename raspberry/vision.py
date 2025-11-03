import os
os.environ["QT_QPA_PLATFORM"] = "xcb"  # Forzar backend X11 (evita bug de Wayland)

import cv2
import numpy as np


# ────────────────────────────────────────────────────────────────
# 1️⃣ Inicialización de cámara e interfaz
# ────────────────────────────────────────────────────────────────

def initialize_camera(index: int = 0): #por defecto 0 (la cámara integrada). Si se pasa 1, se usa la cámara USB.
    #Inicializa la cámara y devuelve el objeto VideoCapture.

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

    # Rangos de color
    lower_red1, upper_red1 = np.array([0, 120, 70]), np.array([10, 255, 255])
    lower_red2, upper_red2 = np.array([170, 120, 70]), np.array([180, 255, 255])
    lower_green, upper_green = np.array([36, 50, 70]), np.array([89, 255, 255])
    lower_blue, upper_blue = np.array([90, 60, 70]), np.array([128, 255, 255])

    # Máscaras
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
# 4️⃣ Dibujo y visualización
# ────────────────────────────────────────────────────────────────

def draw_detections(frame: np.ndarray, detections: list):
    """Dibuja rectángulos, texto y líneas divisorias en el frame."""
    height, width = frame.shape[:2]
    region_left = width // 3
    region_right = 2 * width // 3

    # Dibujar zonas guía
    cv2.line(frame, (region_left, 0), (region_left, height), (255, 255, 255), 2)
    cv2.line(frame, (region_right, 0), (region_right, height), (255, 255, 255), 2)

    # Dibujar cada detección
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
# 5️⃣ Interfaz principal
# ────────────────────────────────────────────────────────────────

def run_interface():
    """Ejecuta el flujo principal de captura, detección y visualización."""
    window_name = "Detección de colores"
    cap = initialize_camera(0)
    create_window(window_name)

    print("✅ Cámara inicializada. Presiona ESC o cierra la ventana para salir.")

    # Bucle principal mientras la ventana esté visible
    while cv2.getWindowProperty(window_name, cv2.WND_PROP_VISIBLE) >= 1:
        ret, frame = cap.read()
        if not ret:
            break

        detections = detect_colors(frame)
        frame = draw_detections(frame, detections)

        cv2.imshow(window_name, frame)

        # Salir con ESC
        if cv2.waitKey(1) & 0xFF == 27:
            break

    cap.release()
    cv2.destroyAllWindows()


# ────────────────────────────────────────────────────────────────
# 6️⃣ Punto de entrada
# ────────────────────────────────────────────────────────────────

if __name__ == "__main__":
    run_interface()
