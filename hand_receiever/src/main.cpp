#include <WiFi.h>
#include <esp_now.h>
#include <Servo.h>

#define SERVO1_PIN 13
#define SERVO2_PIN 12
#define SERVO3_PIN 14
#define SERVO4_PIN 27
#define SERVO5_PIN 26

typedef struct struct_message
{
    int potValues[5];
} struct_message;

struct_message incomingData;
Servo servos[5];

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingDataRaw, int len)
{
    memcpy(&incomingData, incomingDataRaw, sizeof(incomingData));

    Serial.print("Receieved Values: ");
    for (int i = 0; i < 5; i++)
    {
        Serial.print(incomingData.potValues[i]);
        Serial.print(i < 4 ? ", " : "\n");
        int angle = map(incomingData.potValues[i], 0, 4095, 0, 180);
        angle = constrain(angle, 0, 180);
        servos[i].write(angle);
    }
}

void setup()
{
    Serial.begin(115200);

    servos[0].attach(SERVO1_PIN);
    servos[1].attach(SERVO2_PIN);
    servos[2].attach(SERVO3_PIN);
    servos[3].attach(SERVO4_PIN);
    servos[4].attach(SERVO5_PIN);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("ESP NOW initilaziton failed...");
        return;
    }
    esp_now_register_recv_cb(OnDataRecv);

    Serial.println("Robotic Hand is ready!");
}

void loop()
{
}
