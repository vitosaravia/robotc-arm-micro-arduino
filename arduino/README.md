# 🤖 Control del Brazo con Arduino

Este directorio contiene los programas desarrollados en **Arduino** para el control de los servomotores y la comunicación con otros dispositivos (como la Raspberry Pi).

## 📂 Estructura

- **control_servos/** → incluye los códigos de prueba y calibración de los servomotores.  
- **comunicacion_serial/** → contiene los programas que gestionan la comunicación entre el microcontrolador y la Raspberry Pi.

## 🧠 Descripción general

El microcontrolador **ATmega328** (Arduino Uno/Nano) se encarga del control de los actuadores del brazo manipulador.  
Los programas permiten realizar movimientos básicos como levantar, mover y soltar objetos, así como recibir comandos externos.

## ⚙️ Requisitos

- **IDE Arduino** versión 1.8.x o superior  
- Librerías:
  - `Servo.h`
  - `SoftwareSerial.h` (si aplica)
- Placa: Arduino Uno o Nano  
- Comunicación: USB o Serial (Tx/Rx)
