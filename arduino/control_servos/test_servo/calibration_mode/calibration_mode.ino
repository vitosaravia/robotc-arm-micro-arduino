#include <Servo.h>

// --- Declaración de servos ---
Servo servo_1;  // base
Servo servo_2;  // primer brazo
Servo servo_3;  // segundo brazo
Servo servo_4;  // giro Z
Servo servo_5;  // giro X
Servo servo_G;  // apertura del gripper

// --- Posiciones iniciales ---
int Grados_1 = 30;
int Grados_2 = 70;
int Grados_3 = 0;
int Grados_4 = 0;
int Grados_5 = 20;
int Grados_G = 0;

// --- Pines analógicos de joysticks ---
#define Joystick1_X A1
#define Joystick1_Y A0
#define Joystick2_X A2
#define Joystick2_Y A3
#define Joystick3_X A4
#define Joystick3_Y A5

// --- Pines de botones ---
#define button_open 4   // imprime posiciones
#define button_close 5  // cierra gripper

// --- Variable para detectar flanco del botón open ---
bool lastButtonState = HIGH;

void setup() {
  Serial.begin(9600);

  // Asignación de pines PWM
  servo_1.attach(11);   // base
  servo_2.attach(9);   // primer brazo
  servo_3.attach(8);  // segundo brazo
  servo_4.attach(10);  // giro Z
  servo_5.attach(6);   // giro X
  servo_G.attach(3);   // gripper apertura

  pinMode(button_open, INPUT_PULLUP);
  pinMode(button_close, INPUT_PULLUP);

  // Posición inicial
  servo_1.write(Grados_1);
  servo_2.write(Grados_2);
  servo_3.write(Grados_3);
  servo_4.write(Grados_4);
  servo_5.write(Grados_5);
  servo_G.write(Grados_G);
}

void loop() {
  // --- Lectura de todos los joysticks ---
  int J1X = analogRead(Joystick1_X);
  int J1Y = analogRead(Joystick1_Y);
  int J2X = analogRead(Joystick2_X);
  int J2Y = analogRead(Joystick2_Y);
  int J3X = analogRead(Joystick3_X);
  int J3Y = analogRead(Joystick3_Y);

  // --- Joystick 1 controla base y primer brazo ---
  if (J1X < 340) Grados_1 += 3;
  else if (J1X > 680) Grados_1 -= 3;

  if (J1Y < 340) Grados_2 += 3;
  else if (J1Y > 680) Grados_2 -= 3;

  // --- Joystick 2 controla segundo brazo y giro Z ---
  if (J2X < 340) Grados_3 += 3;
  else if (J2X > 680) Grados_3 -= 3;

  if (J2Y < 340) Grados_4 += 3;
  else if (J2Y > 680) Grados_4 -= 3;

  // --- Joystick 3 controla giro X y apertura del gripper ---
  if (J3X < 340) Grados_5 += 3;
  else if (J3X > 680) Grados_5 -= 3;

  if (J3Y < 380) Grados_G += 1;   // abrir gripper
  else if (J3Y > 580) Grados_G -= 1; // cerrar gripper

  // --- Control con botón adicional para cerrar pinza ---
  if (digitalRead(button_close) == LOW) {
    Grados_G -= 3;
  }

  // --- Limitar rangos ---
  Grados_1 = constrain(Grados_1, 0, 180);
  Grados_2 = constrain(Grados_2, 0, 180);
  Grados_3 = constrain(Grados_3, 0, 180);  // ✅ corregido
  Grados_4 = constrain(Grados_4, 0, 180);
  Grados_5 = constrain(Grados_5, 0, 180);
  Grados_G = constrain(Grados_G, 20, 120);


  // --- Actualizar servos ---
  servo_1.write(Grados_1);
  servo_2.write(Grados_2);
  servo_3.write(Grados_3);
  servo_4.write(Grados_4);
  servo_5.write(Grados_5);
  servo_G.write(Grados_G);

  // --- Imprimir lista con el botón button_open ---
  bool buttonState = digitalRead(button_open);
  if (buttonState == LOW && lastButtonState == HIGH) {
    Serial.print("[");
    Serial.print("Base="); Serial.print(Grados_1);
    Serial.print(", Brazo1="); Serial.print(Grados_2);
    Serial.print(", Brazo2="); Serial.print(Grados_3);
    Serial.print(", GiroZ="); Serial.print(Grados_4);
    Serial.print(", GiroX="); Serial.print(Grados_5);
    Serial.print(", Gripper="); Serial.print(Grados_G);
    Serial.println("]");
  }
  lastButtonState = buttonState;

  delay(20);
}
