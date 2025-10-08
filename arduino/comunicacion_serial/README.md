# 🔗 Comunicación Serial Arduino ↔ Raspberry Pi

En esta carpeta se encuentran los programas encargados de la **comunicación entre el Arduino y la Raspberry Pi**.

## 🧩 Funcionalidad

- Recibir comandos desde la Raspberry Pi (por ejemplo, detección de color).  
- Enviar retroalimentación al sistema principal.  
- Ejecutar movimientos específicos del brazo según los comandos recibidos.

## 💡 Tecnologías utilizadas

- Protocolo **UART / Serial**
- Librería `SoftwareSerial.h`
- Baud rate recomendado: **9600 - 115200**

## 🧠 Ejemplo de mensaje
```text
COLOR=ROJO
ACCION=AGARRAR
