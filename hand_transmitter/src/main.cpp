#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

const int potvalues[5] = {14, 25, 27, 32, 35};

uint8_t recv_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

typedef struct __attribute__((packed))
{
    int potValue[5];
} potData_t;

void potDataSend()
{
    static unsigned long lastSendTime = 0;
    if (millis() - lastSendTime > 100)
    {
        lastSendTime = millis();
        potData_t data;

        for (int i = 0; i < 5; i++)
        {
            data.potValue[i] = analogRead(potvalues[i]);
        }
        esp_now_send(recv_mac, (uint8_t *)&data, sizeof(data));
    }
}

void addPeer()
{
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, recv_mac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
    }
}

void setup()
{
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    esp_now_init();
    addPeer();
}

void loop()
{
    potDataSend();
}