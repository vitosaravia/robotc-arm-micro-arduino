# 🦾 Brazo Manipulador de Bajo Costo con Detección de Colores

## 📘 Descripción del Proyecto
Este proyecto propone el **diseño y desarrollo de un brazo manipulador de bajo costo**, construido con **servomotores** y controlado por un **microcontrolador ATmega328 (Arduino)**.  
El sistema será capaz de **levantar, mover y soltar objetos livianos**, integrando además una etapa de **automatización mediante detección de colores**, permitiendo al robot identificar y manipular objetos según su color.

El objetivo principal es **construir un prototipo funcional, accesible y educativo**, que permita comprender los fundamentos de la robótica aplicada, la mecatrónica y la visión por computadora.

---

## ⚙️ Etapas del Proyecto

### 1️⃣ Construcción del Brazo
- Diseño mecánico del brazo manipulador utilizando materiales accesibles (acrílico, MDF o impresión 3D).  
- Ensamblaje de los **servomotores** en las articulaciones para permitir movimientos en varios grados de libertad.  
- Instalación del microcontrolador y componentes electrónicos de control.

### 2️⃣ Programación con Arduino
- Control individual y coordinado de los **servomotores** mediante señales PWM.  
- Implementación de funciones básicas: levantar, mover y soltar objetos.  
- Desarrollo de rutinas de calibración y limitación de ángulos de movimiento.

### 3️⃣ Programación en Raspberry Pi 3
- Implementación de un sistema de **detección de colores** mediante una cámara USB o módulo de cámara.  
- Procesamiento de imágenes con **Python y OpenCV** para identificar el color del objeto.  
- Comunicación con Arduino a través de puerto serial para enviar comandos de acción.

### 4️⃣ Cinemática del Brazo
- Cálculo de la **cinemática directa e inversa** del brazo para posicionar el efector final según la ubicación detectada del objeto.  
- Coordinación entre visión, cálculo y movimiento para lograr una manipulación autónoma.

---

## 🧩 Tecnologías y Componentes
- **Microcontrolador:** ATmega328 (Arduino Uno o Nano)  
- **Controlador principal:** Raspberry Pi 3  
- **Lenguajes:** C++ (Arduino), Python (Raspberry Pi)  
- **Librerías:**  
  - `Servo.h` (control de servos en Arduino)  
  - `OpenCV` (procesamiento de imagen en Raspberry Pi)  
  - `Serial` / `pySerial` (comunicación entre dispositivos)  
- **Actuadores:** Servomotores SG90 / MG995  
- **Sensores:** Cámara USB o cámara del módulo Raspberry Pi  
- **Alimentación:** 5V DC  

---

## 🚀 Objetivos del Proyecto
- Diseñar un **sistema robótico didáctico y accesible**.  
- Comprender los fundamentos de **control de movimiento** mediante servomotores.  
- Aplicar **visión por computadora** para la automatización de tareas.  
- Integrar hardware y software en un entorno funcional.

---

## 📂 Estructura del Repositorio
```bash
robotic-arm/
│
├── arduino/
│   ├── control_servos/
│   │	├── test-1/
│   │	│   └── test-1.ino
│   │	├── 
│   │	└── 
│   └── comunicacion_serial/
│
├── planos-cad/
│   ├── archivos_cad/
│   └── instrucciones_pap/
│
├── raspberry/
│   ├── deteccion_color.py
│   └── control_brazo.py
│
├── docs/
│   ├── esquema_electrico.png
│   └── diagrama_cinematico.pdf
│
└── README.md

