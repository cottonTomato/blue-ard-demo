#include <Arduino.h>
#include <BluetoothSerial.h>

#define DEVICE_NAME "ESP32-Abhi"

BluetoothSerial SerialBT;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting Classic BT");
  SerialBT.begin(DEVICE_NAME, true);
  Serial.println("Bluetooht Setup Complete");

  // SerialBT.register_callback([](esp_spp_cb_event_t event, esp_spp_cb_param_t *param) {
  //   switch (event)
  //   {
  //   case ESP_SPP_SRV_OPEN_EVT:
  //     Serial.println("Client Connected");
  //     break;
    
  //   default:
  //     break;
  //   }
  // });
}

void loop()
{
  if (SerialBT.hasClient())
  {
    if (SerialBT.available())
    {
      Serial.write(SerialBT.read());
    }
    if (Serial.available())
    {
      SerialBT.write(Serial.read());
    }
  } else {
    SerialBT.connect();
  }
  delay(100);
}
