// ────────────────────────────────────────────────
// 🔹 Comunicación Serial con Raspberry Pi
//    Recibe letras: L, C, R, N
//    y las muestra por el monitor serial.
// ────────────────────────────────────────────────

char incomingChar;    // Variable para guardar el carácter recibido
String message = "";  // Para almacenar la línea completa

void setup() {
  Serial.begin(9600);       // Velocidad del puerto (debe coincidir con Python)
  Serial.println("🚀 Esperando señales desde la Raspberry...");
}

void loop() {
  // Verifica si hay datos disponibles en el puerto serial
  while (Serial.available() > 0) {
    incomingChar = Serial.read();  // Lee un byte (carácter)

    if (incomingChar == '\n') {    // Si llega un salto de línea → mensaje completo
      processMessage(message);
      message = "";                // Limpia el buffer
    } 
    else {
      message += incomingChar;     // Acumula los caracteres
    }
  }
}

// ────────────────────────────────────────────────
// 🔸 Función para procesar el mensaje recibido
// ────────────────────────────────────────────────
void processMessage(String msg) {
  msg.trim();  // Elimina espacios y saltos de línea extra

  if (msg == "L") {
    Serial.println("⬅️  Objeto detectado a la IZQUIERDA");
  } 
  else if (msg == "C") {
    Serial.println("⬆️  Objeto en el CENTRO");
  } 
  else if (msg == "R") {
    Serial.println("➡️  Objeto a la DERECHA");
  } 
  else if (msg == "N") {
    Serial.println("❌ Ningún objeto detectado");
  } 
  else {
    Serial.print("⚠️ Señal desconocida: ");
    Serial.println(msg);
  }
}
