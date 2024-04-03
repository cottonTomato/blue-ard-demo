#include <Arduino.h>
#include <BluetoothSerial.h>

#define SERVER_NAME "ESP32-Abhi"
#define CLIENT_NAME "ESP32-Adi"

#define NUM_LEDS 4
#define LED_STATE_SIZE (NUM_LEDS * sizeof(bool))

bool ledState[NUM_LEDS] = { false, false, false, false };
uint8_t ledSelect = NUM_LEDS - 1; // Starting at Last Field so that the first increment will be 0

BluetoothSerial SerialBT;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting Classic BT");
  SerialBT.begin(CLIENT_NAME, true);
  Serial.println("Bluetooth Setup Complete");
}

void loop()
{
  if (SerialBT.hasClient()) {
    ledSelect = (ledSelect + 1) % NUM_LEDS;
    ledState[ledSelect] = !ledState[ledSelect];
    SerialBT.write((uint8_t *)ledState, LED_STATE_SIZE);
  }
  delay(1000);
}
