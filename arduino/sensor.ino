#define LED_PIN 2
#define BUTTON_PIN 4

// IDs de sensores simulados
const char* SENSOR_TEMP_ID = "TEMP";
const char* SENSOR_PRES_ID = "PRES";

// Contador de ciclos
int ciclo = 0;

void setup() {
  // Inicializar puerto serial
  Serial.begin(9600); // Cambiar a 115200 si es necesario
  
  // Configurar pines
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Mensaje de bienvenida
  delay(1000);
  Serial.println("\n========================================");
  Serial.println("ESP32 - Simulador de Sensores IoT");
  Serial.println("Formato: ID:valor");
  Serial.println("Ejemplo: TEMP:25.5");
  Serial.println("========================================");
  Serial.println("");
}

void loop() {
  ciclo++;
  
  // Generar valores simulados de temperatura (20-40°C)
  float temperatura = 20.0 + (ciclo % 20) + (random(0, 100) / 100.0);
  
  // Generar valores simulados de presión (95-105 kPa)
  int presion = 95 + (ciclo % 10);
  
  // Enviar lectura de temperatura
  Serial.print(SENSOR_TEMP_ID);
  Serial.print(":");
  Serial.println(temperatura);
  
  // Parpadear LED para indicar transmisión
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  
  delay(500);
  
  // Enviar lectura de presión
  Serial.print(SENSOR_PRES_ID);
  Serial.print(":");
  Serial.println(presion);
  
  // Parpadear LED
  digitalWrite(LED_PIN, HIGH);
  delay(100);
  digitalWrite(LED_PIN, LOW);
  
  // Esperar 1 segundo entre ciclos
  delay(1000);
  
  // Verificar si se presionó el botón para salir
  if (digitalRead(BUTTON_PIN) == LOW) {
    delay(50); // Debounce
    if (digitalRead(BUTTON_PIN) == LOW) {
      Serial.println("\nSimulación pausada. Presione el botón nuevamente para continuar.");
      while (digitalRead(BUTTON_PIN) == LOW) {
        delay(10);
      }
      delay(50);
    }
  }
}
