void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    char signal = Serial.read();

    if (signal == 'L') {
      Serial.println("Rojo a la IZQUIERDA");
    } else if (signal == 'C') {
      Serial.println("Rojo al CENTRO");
    } else if (signal == 'R') {
      Serial.println("Rojo a la DERECHA");
    } else if (signal == 'N') {
      Serial.println("No detectado");
    }
  }
}
