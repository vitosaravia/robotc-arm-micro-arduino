#include <Servo.h>

// --- Declaración de servos ---
Servo servo_1;  // base
Servo servo_2;  // primer brazo
Servo servo_3;  // segundo brazo
Servo servo_4;  // giro Z
Servo servo_5;  // giro X
Servo servo_G;  // apertura del gripper

// --- Estructura para una posición ---
struct Posicion {
  const char* nombre;
  int Base;
  int Brazo1;
  int Brazo2;
  int GiroZ;
  int GiroX;
  int Gripper;
};

// --- Definición de posiciones ---
Posicion posiciones[] = {
  {"Home", 50, 100, 0, 0, 165, 105},
  {"izq_1", 65, 60, 0, 35, 180, 90},
  {"izq_2", 65, 25, 0, 0, 120, 120},
  {"Home", 50, 100, 0, 0, 165, 105}
};

// Cantidad de posiciones
const int numPosiciones = sizeof(posiciones) / sizeof(Posicion);

// --- Función para mover un servo suavemente ---
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

// --- Mover el brazo completo hacia una posición ---
void moverBrazo(Posicion pos, int velocidad) {
  Serial.print("➡️ Moviendo a "); Serial.println(pos.nombre);

  moverServoSuave(servo_1, servo_1.read(), pos.Base, velocidad);
  moverServoSuave(servo_2, servo_2.read(), pos.Brazo1, velocidad);
  moverServoSuave(servo_3, servo_3.read(), pos.Brazo2, velocidad);
  moverServoSuave(servo_4, servo_4.read(), pos.GiroZ, velocidad);
  moverServoSuave(servo_5, servo_5.read(), pos.GiroX, velocidad);
  moverServoSuave(servo_G, servo_G.read(), pos.Gripper, velocidad);

  Serial.print("✅ Llegó a "); Serial.println(pos.nombre);
}

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando secuencia de movimiento...");

  // Asignación de pines PWM a cada servo
  servo_1.attach(11);   // base
  servo_2.attach(9);    // primer brazo
  servo_3.attach(8);    // segundo brazo
  servo_4.attach(10);   // giro Z
  servo_5.attach(6);    // giro X
  servo_G.attach(3);    // gripper

  // Mover todos los servos a la posición inicial (Home)
  servo_1.write(posiciones[0].Base);
  servo_2.write(posiciones[0].Brazo1);
  servo_3.write(posiciones[0].Brazo2);
  servo_4.write(posiciones[0].GiroZ);
  servo_5.write(posiciones[0].GiroX);
  servo_G.write(posiciones[0].Gripper);

  delay(2000); // esperar un poco antes de comenzar
}

void loop() {
  for (int i = 0; i < numPosiciones; i++) {
    moverBrazo(posiciones[i], 15); // velocidad: 10–20 = suave, menor = más rápido
    delay(1000);                   // pausa entre movimientos
  }

  Serial.println("🔁 Secuencia completa. Reiniciando...");
  delay(10000); // espera antes de repetir
}
