// Este codigo es el primero donde se toma el valor VRx del joystick para mover el servo-1
// falta el control de un servo, que puede ser el de la pinza
// este codigo es por control de joystick, todavia no esta adaptado a la comunicacion serial

//Ambos se alimentan con 4 pilas AA que entregan 5V, GND a la placa.



#include <Servo.h>

Servo servo_1;  //servo_1 -> joystick 1 X -> base rotativa
Servo servo_2;  //servo_2 -> joystick 1 Y -> 1er brazo
Servo servo_3;  //servo_3 -> joystick 2 X -> 2do brazo
Servo servo_4;  //servo_4 -> joystick 2 Y -> giro gripper
Servo servo_5;  //servo_5 -> gripper

int Grados_1 = 0;  // posición servo_1
int Grados_2 = 0;  // posición servo_2
int Grados_3 = 0;  // posición servo_3
int Grados_4 = 0;  // posición servo_4
int Grados_5 = 0;  // posición servo_5

#define Joystick1_X A0  // entrada analogica Joystick1 X
#define Joystick1_Y A1  // entrada analogica Joystick1 Y
#define Joystick2_X A2  // entrada analogica Joystick1 X
#define Joystick2_Y A3  // entrada analogica Joystick1 Y
#define button_gripper 8  // Pin donde está conectado el botón que activa el gripper

void setup() {
  Serial.begin(9600);
  servo_1.attach(9);   // salida pwm pin 9
  servo_2.attach(6);   // salida pwm pin 6
  servo_3.attach(10);   // salida pwm pin 10
  servo_4.attach(11);   // salida pwm pin 11
  servo_5.attach(5);   // salida pwm pin 5

  pinMode(button_gripper, INPUT);
}

void loop() {
  int Joystick1_X_valor = analogRead(Joystick1_X);
  int Joystick1_Y_valor = analogRead(Joystick1_Y);

  int Joystick2_X_valor = analogRead(Joystick2_X);
  int Joystick2_Y_valor = analogRead(Joystick2_Y);

  if (Joystick1_X_valor < 340) Grados_1 += 3;
  else if (Joystick1_X_valor > 680) Grados_1 -= 3;
  
  if (Joystick1_Y_valor < 340) Grados_2 += 3;
  else if (Joystick1_Y_valor > 680) Grados_2 -= 3;

  if (Joystick2_X_valor < 340) Grados_3 += 3;
  else if (Joystick2_X_valor > 680) Grados_3 -= 3;
  
  if (Joystick2_Y_valor < 340) Grados_4 += 3;
  else if (Joystick2_Y_valor > 680) Grados_3 -= 3;

  // Rangos de los servos
  Grados_1 = min(180, max(0, Grados_1));  // Angulos maximos y minimos de servo 1
  Grados_2 = min(180, max(0, Grados_2));  // Angulos maximos y minimos de servo 2

  Grados_3 = min(180, max(0, Grados_3));  // Angulos maximos y minimos de servo 3
  Grados_4 = min(180, max(0, Grados_4));  // Angulos maximos y minimos de servo 4


  // Leer el botón
  int estadoGripper = digitalRead(button_gripper);

  // Si el botón se presiona (LOW), mover el servo_5 a 60°
  if (estadoGripper == LOW) {
    Grados_5 = 60;
  } else {
    Grados_5 = 0;
  }
 
    // Monitorear por Serial
  Serial.print("Grados eje 1 : ");
  Serial.println(Grados_1); 
  Serial.print("Grados eje 2 : ");
  Serial.println(Grados_2);  
  Serial.print("Grados eje 3 : ");
  Serial.println(Grados_3); 
  Serial.print("Grados eje 4 : ");
  Serial.println(Grados_4);  
  Serial.print("Grados eje 5: ");
  Serial.println(Grados_5);
  Serial.print("\n");
  
  servo_1.write(Grados_1);
  servo_2.write(Grados_2);
  servo_3.write(Grados_3);
  servo_4.write(Grados_4);
  servo_4.write(Grados_5);

  delay(50);  // Pequeña pausa para estabilidad
  
}
