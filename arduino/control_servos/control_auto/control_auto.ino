#include <Servo.h>

// --- Declaración de servos ---
Servo servo_1;  // base
Servo servo_2;  // primer brazo
Servo servo_3;  // segundo brazo
Servo servo_4;  // giro Z
Servo servo_5;  // giro X
Servo servo_G;  // apertura del gripper

// --- Estructura de posición ---
struct Posicion {
  const char* nombre;
  int Base;
  int Brazo1;
  int Brazo2;
  int GiroZ;
  int GiroX;
  int Gripper;
};

// --- Secuencia IZQUIERDA ---
Posicion posiciones_izq[] = {
  {"Home", 50, 100, 0, 0, 165, 105},
  {"izq_1", 65, 60, 0, 0, 180, 90},
  {"izq_2", 69, 25, 0, 0, 150, 120},
  {"izq_agarre_abierto", 69, 36, 0, 0, 150, 120},
  {"izq_agarre_cerrado", 69, 36, 0, 0, 155, 20},
  {"izq_3", 50, 100, 0, 0, 165, 20},
  {"izq_4", 180, 75, 0, 0, 123, 120},
  {"Home", 50, 100, 0, 0, 165, 105}
};

// --- Secuencia CENTRO ---
Posicion posiciones_cen[] = {
  {"Home", 50, 100, 0, 0, 165, 105},
  {"izq_1", 25, 60, 0, 0, 180, 90},
  {"izq_2", 25, 25, 0, 0, 150, 120},
  {"izq_agarre_abierto", 25, 36, 0, 0, 150, 120},
  {"izq_agarre_cerrado", 25, 36, 0, 0, 155, 20},
  {"izq_3", 25, 100, 0, 0, 165, 20},
  {"izq_4", 180, 75, 0, 0, 123, 120},
  {"Home", 50, 100, 0, 0, 165, 105}
};

// --- Secuencia DERECHA ---
Posicion posiciones_der[] = {
  {"Home", 50, 100, 0, 0, 165, 105},
  {"izq_1", 0, 60, 0, 0, 180, 90},
  {"izq_2", 0, 25, 0, 0, 150, 120},
  {"izq_agarre_abierto", 0, 36, 0, 0, 150, 120},
  {"izq_agarre_cerrado", 0, 36, 0, 0, 155, 20},
  {"izq_3", 0, 100, 0, 0, 165, 20},
  {"izq_4", 180, 75, 0, 0, 123, 120},
  {"Home", 50, 100, 0, 0, 165, 105}
};

// --- Función para mover servos suavemente ---
void moverServoSuave(Servo &servo, int desde, int hasta, int velocidad) {
  if (desde < hasta) {
    for (int i = desde; i <= hasta; i++) {
      servo.write(i);
      delay(velocidad);
    }
  } else {
    for (int i = desde; i >= hasta; i--) {
      servo.write(i);
      delay(velocidad);
    }
  }
}

// --- Mueve el brazo completo a una posición ---
void moverBrazo(Posicion pos, int velocidad) {
  Serial.print("➡️ Moviendo a: ");
  Serial.println(pos.nombre);

  moverServoSuave(servo_1, servo_1.read(), pos.Base, velocidad);
  moverServoSuave(servo_2, servo_2.read(), pos.Brazo1, velocidad);
  moverServoSuave(servo_3, servo_3.read(), pos.Brazo2, velocidad);
  moverServoSuave(servo_4, servo_4.read(), pos.GiroZ, velocidad);
  moverServoSuave(servo_5, servo_5.read(), pos.GiroX, velocidad);
  moverServoSuave(servo_G, servo_G.read(), pos.Gripper, velocidad);

  Serial.print("✅ Llegó a: ");
  Serial.println(pos.nombre);
}

// --- Ejecuta una secuencia completa ---
void ejecutarSecuencia(Posicion secuencia[], int cantidad, const char* nombre) {
  Serial.print("\n▶️ Ejecutando secuencia: ");
  Serial.println(nombre);
  for (int i = 0; i < cantidad; i++) {
    moverBrazo(secuencia[i], 15);
    delay(1000);
  }
  Serial.println("🏁 Secuencia finalizada\n");
}

void setup() {
  Serial.begin(9600);
  Serial.println("✅ Sistema listo. Escribe: izq / cen / der");

  servo_1.attach(11);
  servo_2.attach(9);
  servo_3.attach(8);
  servo_4.attach(10);
  servo_5.attach(6);
  servo_G.attach(3);

  // posición inicial
  servo_1.write(50);
  servo_2.write(100);
  servo_3.write(0);
  servo_4.write(0);
  servo_5.write(165);
  servo_G.write(105);
}

void loop() {
  // Si hay datos en el monitor serial...
  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();  // eliminar espacios y saltos de línea

    if (comando == "izq") {
      ejecutarSecuencia(posiciones_izq, sizeof(posiciones_izq) / sizeof(Posicion), "IZQUIERDA");
    } 
    else if (comando == "cen") {
      ejecutarSecuencia(posiciones_cen, sizeof(posiciones_cen) / sizeof(Posicion), "CENTRO");
    } 
    else if (comando == "der") {
      ejecutarSecuencia(posiciones_der, sizeof(posiciones_der) / sizeof(Posicion), "DERECHA");
    } 
    else {
      Serial.println("❌ Comando no reconocido. Usa: izq / cen / der");
    }
  }
}
