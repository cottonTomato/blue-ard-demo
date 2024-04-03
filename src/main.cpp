#include <Arduino.h>
#include <BluetoothSerial.h>

#define CLIENT_NAME "ESP32-Adi"
#define SERVER_NAME "ESP32-Abhi"

#define NUM_LEDS 4
#define LED_STATE_SIZE (NUM_LEDS * sizeof(bool))

uint8_t ledPins[NUM_LEDS] = {32, 33, 34, 35};

BluetoothSerial SerialBT;

void setup()
{
  Serial.begin(115200);

  Serial.println("Setting up LED Pins");
  for (int i = 0; i < NUM_LEDS; i++)
  {
    uint8_t pin = ledPins[i];
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  Serial.println("Starting Classic BT");
  SerialBT.begin(CLIENT_NAME, false);
  Serial.println("Bluetooth Setup Complete");

  Serial.println("Attaching Callback on Data Recieve");
  SerialBT.onData([](const uint8_t *buffer, size_t size)
                  {
    if (size != LED_STATE_SIZE) {
      return;
    }
    for (int i = 0; i < NUM_LEDS; i++) {
      Serial.print(buffer[i] ? "1" : "0");
      uint8_t pin = ledPins[i];
      digitalWrite(pin, buffer[i] ? HIGH : LOW);
    } });
}

void loop()
{
  if (!SerialBT.hasClient())
  {
    SerialBT.connect(SERVER_NAME);
  }
  delay(1000);
}
