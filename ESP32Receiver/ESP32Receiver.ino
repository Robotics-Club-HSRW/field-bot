//MAC: 08:3a:8d:90:25:8c
#include <ESP32Servo.h>
#include <esp_now.h>
#include <WiFi.h>

// Structure example to send data
typedef struct struct_message {
  int x;
  int y;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("Int X: ");
  // Serial.println(myData.x);
  // Serial.print("Int Y: ");
  // Serial.println(myData.y);
  // Serial.println();
}

#define ESC1_PIN 25
#define ESC2_PIN 26
Servo esc1;
Servo esc2;

void setup() {
  esc1.attach(ESC1_PIN, 1000, 2000);
  esc2.attach(ESC2_PIN, 1000, 2000);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the receive callback
  esp_now_register_recv_cb(OnDataRecv);
  delay(1000);
}

void loop() {
  int joyposVert = myData.x;
  int joyposHorz = myData.y;

  Serial.print(joyposVert);
  Serial.print("  |  ");
  Serial.println(joyposHorz);

  int motorF = map(joyposVert, 1000, 0, 0, 180);
  int motorB = map(joyposVert, 3000, 4095, 0, 180);
  // Control the motors
  esc1.write(motorF); // forward motion
  esc2.write(motorB); // backward motion

  if (joyposHorz > 2800) {
    int turn = map(joyposHorz, 3000, 4095, 0, 180);
    esc1.write(turn);
    esc2.write(turn);
    delay(10);
  }
  delay(10); // Add a delay to avoid overwhelming the server
}

// by luko & tsecret