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

// --- Secuencias predefinidas ---
Posicion posiciones_izq[] = {
  {"Home", 50, 100, 0, 0, 165, 105},
  {"izq_1", 65, 60, 0, 0, 180, 90},
  {"izq_2", 69, 25, 0, 0, 150, 120},
  {"izq_agarre_abierto", 69, 36, 0, 0, 150, 120},
  {"izq_agarre_cerrado", 69, 36, 0, 0, 155, 20},
  {"izq_3", 50, 100, 0, 0, 165, 20},
  {"izq_4", 180, 100, 0, 0, 123, 120},
  {"Home", 50, 100, 0, 0, 165, 105}
};

Posicion posiciones_cen[] = {
  {"Home", 50, 100, 0, 0, 165, 105},
  {"cen_1", 25, 60, 0, 0, 180, 90},
  {"cen_2", 25, 25, 0, 0, 150, 120},
  {"cen_agarre_abierto", 25, 36, 0, 0, 150, 120},
  {"cen_agarre_cerrado", 25, 36, 0, 0, 155, 20},
  {"cen_3", 25, 100, 0, 0, 165, 20},
  {"cen_4", 180, 100, 0, 0, 123, 120},
  {"Home", 50, 100, 0, 0, 165, 105}
};

Posicion posiciones_der[] = {
  {"Home", 50, 100, 0, 0, 165, 105},
  {"der_1", 0, 60, 0, 0, 180, 90},
  {"der_2", 0, 25, 0, 0, 150, 120},
  {"der_agarre_abierto", 0, 36, 0, 0, 150, 120},
  {"der_agarre_cerrado", 0, 36, 0, 0, 155, 20},
  {"der_3", 0, 100, 0, 0, 165, 20},
  {"der_4", 180, 100, 0, 0, 123, 120},
  {"Home", 50, 100, 0, 0, 165, 105}
};

// --- Estado ---
bool ejecutandoSecuencia = false;
bool enHome = true;          // 🟢 El brazo solo acepta nuevos comandos estando en Home
char comandoActual = 'N';    // último comando recibido

// --- Configuración inicial ---
void setup() {
  Serial.begin(9600);
  Serial.println("🤖 Esperando comandos seriales (L / C / R)...");

  servo_1.attach(11);
  servo_2.attach(9);
  servo_3.attach(8);
  servo_4.attach(10);
  servo_5.attach(6);
  servo_G.attach(3);

  moverHome();
}

// --- Mover suavemente un servo ---
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

// --- Mover brazo completo a una posición ---
void moverBrazo(Posicion pos, int velocidad) {
  Serial.print("➡️ Moviendo a: ");
  Serial.println(pos.nombre);
  moverServoSuave(servo_1, servo_1.read(), pos.Base, velocidad);
  moverServoSuave(servo_2, servo_2.read(), pos.Brazo1, velocidad);
  moverServoSuave(servo_3, servo_3.read(), pos.Brazo2, velocidad);
  moverServoSuave(servo_4, servo_4.read(), pos.GiroZ, velocidad);
  moverServoSuave(servo_5, servo_5.read(), pos.GiroX, velocidad);
  moverServoSuave(servo_G, servo_G.read(), pos.Gripper, velocidad);
}

// --- Ejecutar secuencia ---
void ejecutarSecuencia(Posicion secuencia[], int cantidad) {
  ejecutandoSecuencia = true;
  enHome = false;  // 🔴 No aceptar nuevos comandos
  for (int i = 0; i < cantidad; i++) {
    moverBrazo(secuencia[i], 10);
    delay(500);
  }
  ejecutandoSecuencia = false;
  enHome = true;   // 🟢 Listo para aceptar nuevas señales
  Serial.println("✅ Secuencia finalizada (Home alcanzada).");
  Serial.println("🏠 Brazo disponible para nuevo comando.\n");
}

// --- Mover a Home directamente ---
void moverHome() {
  servo_1.write(50);
  servo_2.write(100);
  servo_3.write(0);
  servo_4.write(0);
  servo_5.write(165);
  servo_G.write(105);
  delay(1000);
  enHome = true;
}

// --- Bucle principal ---
void loop() {
  // Solo procesar comandos si estamos en Home y no ejecutando
  if (Serial.available() > 0 && !ejecutandoSecuencia && enHome) {
    char comando = Serial.read();
    comando = toupper(comando); // por si llega minúscula

    // Ignora comandos repetidos
    if (comando == comandoActual) return;
    comandoActual = comando;

    Serial.print("🛰 Comando recibido: ");
    Serial.println(comando);

    if (comando == 'L') {
      ejecutarSecuencia(posiciones_izq, sizeof(posiciones_izq) / sizeof(Posicion));
    }
    else if (comando == 'C') {
      ejecutarSecuencia(posiciones_cen, sizeof(posiciones_cen) / sizeof(Posicion));
    }
    else if (comando == 'R') {
      ejecutarSecuencia(posiciones_der, sizeof(posiciones_der) / sizeof(Posicion));
    }
    else if (comando == 'N') {
      Serial.println("⚠️ Ningún objeto detectado. Manteniendo posición.");
    }
  }
}
