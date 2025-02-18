#include <ArduinoJson.h>

// Definicje pinów UART
#define NANO1_RX 36
#define NANO1_TX 35
#define NANO2_RX 38
#define NANO2_TX 37

// Piny sterujące (do ustalenia)
#define SYNC_NANO1 4  // przykładowy pin
#define SYNC_NANO2 5  // przykładowy pin

// Struktura do przechowywania danych z sond
struct SensorData {
  int32_t values[16];
  bool isUpdated;
};

SensorData sensorData;
JsonDocument doc1, doc2;

void setup() {
  Serial.begin(115200);  // Port debug
  
  // Konfiguracja portów szeregowych dla NANO
  Serial1.begin(115200, SERIAL_8N1, NANO1_RX, NANO1_TX);
  Serial2.begin(115200, SERIAL_8N1, NANO2_RX, NANO2_TX);
  
  // Konfiguracja pinów sterujących
  pinMode(SYNC_NANO1, OUTPUT);
  pinMode(SYNC_NANO2, OUTPUT);
  
  digitalWrite(SYNC_NANO1, LOW);
  digitalWrite(SYNC_NANO2, LOW);
  
  sensorData.isUpdated = false;
  
  // Wyczyść bufor
  clearSensorData();
}

void loop() {
  static bool nano1Ready = false;
  static bool nano2Ready = false;
  
  // Sprawdź dane z NANO1
  if (Serial1.available()) {
    DeserializationError err = deserializeJson(doc1, Serial1);
    if (!err) {
      JsonArray data = doc1["data"];
      for (int i = 0; i < 8; i++) {
        sensorData.values[i] = data[i];
      }
      nano1Ready = true;
    }
  }
  
  // Sprawdź dane z NANO2
  if (Serial2.available()) {
    DeserializationError err = deserializeJson(doc2, Serial2);
    if (!err) {
      JsonArray data = doc2["data"];
      for (int i = 0; i < 8; i++) {
        sensorData.values[i + 8] = data[i];
      }
      nano2Ready = true;
    }
  }
  
  // Jeśli mamy dane z obu NANO
  if (nano1Ready && nano2Ready) {
    printSensorData();
    
    // Zasygnalizuj gotowość do następnego cyklu
    digitalWrite(SYNC_NANO1, HIGH);
    digitalWrite(SYNC_NANO2, HIGH);
    delayMicroseconds(100);
    digitalWrite(SYNC_NANO1, LOW);
    digitalWrite(SYNC_NANO2, LOW);
    
    // Reset flag
    nano1Ready = false;
    nano2Ready = false;
  }
}

void clearSensorData() {
  for (int i = 0; i < 16; i++) {
    sensorData.values[i] = 0;
  }
}

void printSensorData() {
  Serial.println("{");
  for (int i = 0; i < 16; i++) {
    Serial.print("  S");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(sensorData.values[i]);
    if (i < 15) Serial.println(",");
    else Serial.println();
  }
  Serial.println("}");
}