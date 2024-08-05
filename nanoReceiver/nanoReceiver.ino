// #include <WiFi.h>
// #include <WiFiClient.h>
// #include <WiFiServer.h>
#include <SPI.h>
#include <Servo.h>
// // Replace with your network credentials
// const char* ssid = "YOUR_SSID";
// const char* password = "YOUR_PASSWORD";

// WiFiServer server(80);

// // Server URL to send data
// const char* serverName = "http://yourserver.com/joystick-data"; // Replace with your server URL

#define JoyStick_X_PIN A0
#define JoyStick_Y_PIN A1
int joyposVert;
int joyposHorz;

#define ESC1_PIN 5
#define ESC2_PIN 6
Servo esc1;
Servo esc2;
Servo fin;

void setup() {
  Serial.begin(9600);

 // Connect to Wi-Fi
  // WiFi.begin(ssid, password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi...");
  // }
  // Serial.println("Connected to WiFi");

  //  // Start the server
  // server.begin();

  pinMode(JoyStick_X_PIN, INPUT);
  pinMode(JoyStick_Y_PIN, INPUT);

  // Initialize servos
  fin.attach(3);
  for (int i = 180; i > 0; i--) {
    fin.write(i);
    delay(10);
  }
  for (int i = 0; i < 180; i++) {
    fin.write(i);
    delay(10);
  }

  esc1.attach(ESC1_PIN, 1000, 2000);
  esc1.write(180);
  delay(5000);
  esc1.write(0);
  delay(2000);
  esc1.write(10);

  esc2.attach(ESC2_PIN, 1000, 2000);
  esc2.write(180);
  delay(5000);
  esc2.write(0);
  delay(2000);
  esc2.write(10);
}

void loop() {
  int joyposVert = analogRead(JoyStick_X_PIN);
  int joyposHorz = analogRead(JoyStick_Y_PIN);

  Serial.print(joyposVert);
  Serial.print("  |  ");
  Serial.println(joyposHorz);



  int motorF = map(joyposVert, 250, 0, 0, 180);
  int motorB = map(joyposVert, 500, 700, 0, 180);
  int rudder;

  // Control the motors
  esc1.write(motorF);
  esc2.write(motorB);

  // Serial.print(motorF);
  // Serial.print("  |  ");
  // Serial.print(motorB);      



  // if (client) {
  //   Serial.println("New Client Connected");
  //   while (client.connected()) {
  //     if (client.available()) {
  //       String data = client.readStringUntil('\n');
  //       int receivedData[2];

  //       // Parse the received data
  //       sscanf(data.c_str(), "%d %d", &receivedData[0], &receivedData[1]);

  //       int throttleCW = receivedData[0];
  //       int throttleCCW = receivedData[1];

  //       int motorCW = map(throttleCW, 300, 0, 0, 180);
  //       int motorCCW = map(throttleCCW, 300, 0, 0, 180);
  //       int rudder;

  //       // Control the motors
  //       esc1.write(motorCW);
  //       esc2.write(motorCCW);

  //       Serial.print(throttleCW);
  //       Serial.print("  |  ");
  //       Serial.print(motorCW);
  //       Serial.print("  |  ");
  //       Serial.print(throttleCCW);
  //       Serial.print("  |  ");
  //       Serial.println(motorCCW);
  //     }
  //   }
  //   client.stop();
  //   Serial.println("Client Disconnected");
  // laudol}

  delay(50); // Add a delay to avoid overwhelming the server
}
// by luko