#include <Arduino.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEDevice.h>

#define DEVICE_NAME "ESP32-BT"

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR_LEDA_UUID "c54f2186-0d10-44c6-9a91-63a98b675999"
#define CHAR_LEDB_UUID "b03f8c79-d53d-4158-be1d-7d878596db58"

#define LEDA_PIN 26
#define LEDB_PIN 27

class LEDSetResetCallbacks : public BLECharacteristicCallbacks
{
public:
  int pin;

  LEDSetResetCallbacks(int pin)
  {
    this->pin = pin;
  }

  void onWrite(BLECharacteristic *pCharacteristic)
  {
    uint8_t value = pCharacteristic->getValue()[0] == '1' ? HIGH : LOW;
    Serial.printf("Setting Pin %d to %d\n", pin, value);
    digitalWrite(pin, value);
  }
};

void setup()
{
  Serial.begin(115200);

  Serial.println("Setting GPIOs for LED");
  pinMode(LEDA_PIN, OUTPUT);
  pinMode(LEDB_PIN, OUTPUT);
  digitalWrite(LEDA_PIN, LOW);
  digitalWrite(LEDB_PIN, LOW);

  Serial.println("Creating BLE Server");
  BLEDevice::init(DEVICE_NAME);
  BLEServer *pServer = BLEDevice::createServer();

  Serial.println("Creating LED Service");
  BLEService *pService = pServer->createService(SERVICE_UUID);

  Serial.println("Creating LED A & B Characteristics");
  BLECharacteristic *pCharLEDA = pService->createCharacteristic(
      CHAR_LEDA_UUID,
      BLECharacteristic::PROPERTY_WRITE);
  BLECharacteristic *pCharLEDB = pService->createCharacteristic(
      CHAR_LEDB_UUID,
      BLECharacteristic::PROPERTY_WRITE);

  Serial.println("Setting LED A & B Callbacks");
  pCharLEDA->setCallbacks(new LEDSetResetCallbacks(LEDA_PIN));
  pCharLEDB->setCallbacks(new LEDSetResetCallbacks(LEDB_PIN));

  Serial.println("Starting Service");
  pCharLEDA->setValue("0");
  pCharLEDB->setValue("0");
  pService->start();

  Serial.println("Advertising Server");
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop()
{
  delay(1000);
}
