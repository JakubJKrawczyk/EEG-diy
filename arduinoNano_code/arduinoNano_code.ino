#include <ADS1256.h>

// Definicje pinów
#define CS_PIN 10      // Chip Select pin
#define DRDY_PIN 9     // Data Ready pin
#define SYNC_PIN 8     // Synchronization pin

// Liczba kanałów do odczytu
const int numChannels = 8;

// Obiekt ADS1256
ADS1256 adc;

void setup() {
    // Konfiguracja pinu SYNC jako wejście
    pinMode(SYNC_PIN, INPUT);

    // Inicjalizacja portu szeregowego
    Serial.begin(115200);

    // Inicjalizacja ADS1256
    adc.begin(CS_PIN, DRDY_PIN);  // Inicjalizacja z domyślnymi ustawieniami rejestrów

    Serial.println("ADS1256 zainicjalizowany z ustawieniami domyślnymi.");
}

void loop() {
    // Czekaj na aktywację pinu SYNC
    while (digitalRead(SYNC_PIN) == LOW) {
        // Opcjonalne: Informuj, że czekasz na SYNC
        Serial.println("Czekam na aktywację SYNC...");
        delay(100); // Odświeżanie statusu co 100ms
    }

    // Pin SYNC aktywny, rozpocznij odczyty
    float values[numChannels];

    for (int channel = 0; channel < numChannels; channel++) {
        adc.setChannel(channel);          // Ustaw kanał
        values[channel] = adc.readADC();  // Odczytaj wartość z kanału
    }

    // Wyświetl wyniki
    Serial.println("Zsynchronizowane dane z kanałów:");
    for (int i = 0; i < numChannels; i++) {
        Serial.print("Kanał ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(values[i], 6); // Wyświetla wynik z 6 miejscami po przecinku
    }

    delay(500); // Opcjonalne opóźnienie przed kolejnym cyklem
}
