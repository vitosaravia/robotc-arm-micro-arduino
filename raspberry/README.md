# 🍓 Control Inteligente desde Raspberry Pi

Esta carpeta contiene los programas en **Python** ejecutados en la Raspberry Pi, que permiten la detección de colores y el control automatizado del brazo.

## 📄 Archivos principales

- **deteccion_color.py** → usa OpenCV para identificar el color de los objetos mediante una cámara.  
- **control_brazo.py** → envía comandos seriales al Arduino según el color detectado.

## 🧠 Funcionalidad

1. La cámara captura la imagen.  
2. El sistema identifica el color predominante.  
3. Se envía un comando al Arduino.  
4. El brazo realiza la acción correspondiente.

## 🧰 Dependencias


