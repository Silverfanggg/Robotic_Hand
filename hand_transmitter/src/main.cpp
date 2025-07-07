#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

const int pot1 = 14;
const int pot2 = 27;
const int pot3 = 25;
const int pot4 = 32;
const int pot5 = 35;

uint8_t receiverMac[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct struct_message
{
  int potvalues[5];
} struct_message;
struct_message myData;

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("Send Status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "SUCCESS" : "FAILURE");
}

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("ESP NOW initilaziton failed...");
    return;
  }

  esp_now_register_send_cb(onDataSent);
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, receiverMac, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer!");
    return;
  }
}
void loop()
{
  myData.potvalues[0] = analogRead(pot1);
  myData.potvalues[1] = analogRead(pot2);
  myData.potvalues[2] = analogRead(pot3);
  myData.potvalues[3] = analogRead(pot4);
  myData.potvalues[4] = analogRead(pot5);
  esp_now_send(receiverMac, (uint8_t *)&myData, sizeof(myData));

  Serial.print("Sending Values: ");
  for (int i = 0; i < 5; i++)
  {
    Serial.print(myData.potvalues[i]);
    Serial.print(i < 4 ? "," : "\n");
  }
  delay(500);
}
