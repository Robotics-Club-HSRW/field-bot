#include <WiFi.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Server URL to send data
const char* serverName = "http://yourserver.com/joystick-data"; // Replace with your server URL

#define JoyStick_X_PIN 23
#define JoyStick_Y_PIN 22
int joyposVert;
int joyposHorz;

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi.");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected.");

  pinMode(JoyStick_X_PIN, INPUT);
  pinMode(JoyStick_Y_PIN, INPUT);
}

void loop() {
  int joyposVert = analogRead(JoyStick_X_PIN);
  int joyposHorz = analogRead(JoyStick_Y_PIN);

  Serial.print(joyposVert);
  Serial.print("  |  ");
  Serial.println(joyposHorz);

  if (WiFi.status() == WL_CONNECTED) { // Check if the ESP32 is connected to WiFi
    HTTPClient http;

    http.begin(serverName); // Specify the URL
    http.addHeader("Content-Type", "application/json"); // Specify content-type header

    // Create JSON payload
    String jsonPayload = "{\"joystick\": {\"x\": " + String(joyposVert) + ", \"y\": " + String(joyposHorz) + "}}";

    int httpResponseCode = http.POST(jsonPayload); // Send the request

    if (httpResponseCode > 0) {
      String response = http.getString(); // Get the response to the request
      Serial.println(httpResponseCode); // Print return code
      Serial.println(response); // Print request answer
    }
    else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end(); // Free resources
  }
  else {
    Serial.println("Error in WiFi connection");
  }

  delay(50); // Add a delay to avoid overwhelming the server
}
