import os
os.environ["QT_QPA_PLATFORM"] = "xcb"  # Forzar backend X11 en Ubuntu/Wayland

import cv2
import numpy as np


def main():
    """Captura video, detecta colores y muestra posición relativa en una sola ventana."""
    # --- Inicialización de cámara ---
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
        print("❌ No se pudo acceder a la cámara.")
        return

    # --- Crear ventana única y reutilizable ---
    window_name = "Detección de objetos"
    cv2.namedWindow(window_name, cv2.WINDOW_NORMAL)
    cv2.resizeWindow(window_name, 640, 480)

    # --- Leer primer frame para definir regiones ---
    ret, frame = cap.read()
    if not ret:
        print("❌ No se pudo leer un frame inicial.")
        cap.release()
        return

    frame_width = frame.shape[1]
    region_left = frame_width // 3
    region_right = 2 * frame_width // 3

    print("✅ Cámara inicializada. Presiona ESC o cierra la ventana para salir.")

    # --- Bucle principal (se ejecuta mientras la ventana esté abierta) ---
    while cv2.getWindowProperty(window_name, cv2.WND_PROP_VISIBLE) >= 1:
        ret, frame = cap.read()
        if not ret:
            break

        # --- Conversión a HSV ---
        hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

        # --- Definir rangos de colores ---
        lower_red1, upper_red1 = np.array([0, 120, 70]), np.array([10, 255, 255])
        lower_red2, upper_red2 = np.array([170, 120, 70]), np.array([180, 255, 255])
        lower_green, upper_green = np.array([36, 50, 70]), np.array([89, 255, 255])
        lower_blue, upper_blue = np.array([90, 60, 70]), np.array([128, 255, 255])

        # --- Crear máscaras ---
        mask_red = cv2.inRange(hsv, lower_red1, upper_red1) + cv2.inRange(hsv, lower_red2, upper_red2)
        mask_green = cv2.inRange(hsv, lower_green, upper_green)
        mask_blue = cv2.inRange(hsv, lower_blue, upper_blue)

        colors = {
            "Rojo": (mask_red, (0, 0, 255)),
            "Verde": (mask_green, (0, 255, 0)),
            "Azul": (mask_blue, (255, 0, 0))
        }

        # --- Procesar cada color ---
        for name, (mask, color_bgr) in colors.items():
            contours, _ = cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
            for cnt in contours:
                area = cv2.contourArea(cnt)
                if area < 1000:
                    continue

                x, y, w, h = cv2.boundingRect(cnt)
                cx, cy = x + w // 2, y + h // 2

                # Determinar posición relativa
                if cx < region_left:
                    position = "IZQUIERDA"
                elif cx > region_right:
                    position = "DERECHA"
                else:
                    position = "CENTRO"

                cv2.rectangle(frame, (x, y), (x + w, y + h), color_bgr, 2)
                cv2.circle(frame, (cx, cy), 5, color_bgr, -1)
                cv2.putText(frame, f"{name}: {position}", (x, y - 10),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.6, color_bgr, 2)

        # --- Dibujar líneas divisorias ---
        cv2.line(frame, (region_left, 0), (region_left, frame.shape[0]), (255, 255, 255), 2)
        cv2.line(frame, (region_right, 0), (region_right, frame.shape[0]), (255, 255, 255), 2)

        # --- Mostrar frame ---
        cv2.imshow(window_name, frame)

        # --- Salir si se presiona ESC ---
        key = cv2.waitKey(1) & 0xFF
        if key == 27:
            break

    # --- Limpieza ---
    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
