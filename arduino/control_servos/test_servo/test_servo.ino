// Este codigo es el primero donde se toma el valor VRx del joystick para mover el servo-1
// Este codigo es el primero donde se toma el valor VRy del joystick para mover el servo-2


//Ambos se alimentan con 4 pilas AA que entregan 5V, GND a la placa.



#include <Servo.h>

Servo servo_1;  //servo_1
Servo servo_2;  //servo_2
int Grados_1 = 0;  // posición servo_1
int Grados_2 = 0;  // posición servo_2

#define Joystick1_X A0  // entrada analogica Joystick1 X
#define Joystick1_Y A1  // entrada analogica Joystick1 X

void setup() {
  Serial.begin(9600);
  servo_1.attach(9);   // salida pwm pin 9
  servo_2.attach(6);   // salida pwm pin 6
}

void loop() {
  int Joystick1_X_valor = analogRead(Joystick1_X);
  int Joystick1_Y_valor = analogRead(Joystick1_Y);

  if (Joystick1_X_valor < 340) Grados_1 += 3;
  else if (Joystick1_X_valor > 680) Grados_1 -= 3;
  
  if (Joystick1_Y_valor < 340) Grados_2 += 3;
  else if (Joystick1_Y_valor > 680) Grados_2 -= 3;

  Grados_1 = min(180, max(0, Grados_1));  // Angulos maximos y minimos de servo 1
  Grados_2 = min(180, max(0, Grados_2));  // Angulos maximos y minimos de servo 2
 
  Serial.print("Grados eje 1 : ");
  Serial.println(Grados_1); 
  Serial.print("Grados eje 2 : ");
  Serial.println(Grados_2);  
  Serial.print("\n");
  
  servo_1.write(Grados_1);
  
  servo_2.write(Grados_2);
  
}
