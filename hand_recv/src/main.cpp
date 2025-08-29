#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <esp_now.h>

Servo servos[5];
const int servoPins[5] = {10, 11, 12, 13, 14};

typedef struct __attribute__((packed))
{
  int potValue[5];
} potData_t;

potData_t Data;

void DataReceived(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&Data, incomingData, sizeof(Data));
  for (int i = 0; i < 5; i++)
  {
    int range = map(Data.potValue[i], 0, 4095, 0, 180);
    int angle = constrain(range, 0, 180);
    servos[i].write(angle);
  }
}

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  esp_now_init();
  esp_now_register_recv_cb(DataReceived);

  for (int i = 0; i < 5; i++)
  {
    servos[i].attach(servoPins[i]);
  }
}

void loop() {}
