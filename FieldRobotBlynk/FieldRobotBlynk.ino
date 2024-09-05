#include <credentials.h>
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL4WP9Bigae"
#define BLYNK_TEMPLATE_NAME "FieldRobot"
#define BLYNK_AUTH_TOKEN "uuI0AKlXC4XJA5io03xadCEqA4JuiS9e"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>


// #include <Blynk.h>

const char *ssid = "LukoSpot";
const char *password = "Duisbu5.";

int joystick_move_pos, joystick_steer_pos, flash, pic;

BLYNK_WRITE(V0) {
  joystick_move_pos = param.asInt();
}

BLYNK_WRITE(V1) {
  joystick_steer_pos = param.asInt();
}

BLYNK_WRITE(V2) {
  flash = param.asInt();
}

BLYNK_WRITE(V3) {
  pic = param.asInt();
}

#define ESC1_PIN 25
#define ESC2_PIN 26
Servo esc1;
Servo esc2;

#define MOTOR_CALIBRATION_DELAY 3000

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi");

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  Serial.println();

  setupMotors();
  calibrateMotors();

  delay(1000);
}

void loop() {

  Blynk.run();

  int motorF = 2000;
  int motorB = 2000;

  // Control esc1 based on joystick_move_pos
  if (joystick_move_pos > 3000) {
    motorF = map(joystick_move_pos, 2500, 4095, 0, 180); // Adjust mapping range
    esc1.write(motorF);
  } else {
    esc1.write(0); // Stop ESC1 when joystick is pushed upwards
  }

  // Control esc2 based on joystick_steer_pos
  if (joystick_move_pos < 1000) {
    motorB = map(joystick_move_pos, 1500, 0, 0, 180); // Adjust mapping range
    esc2.write(motorB);
  } else {
    esc2.write(0); // Stop ESC2 when joystick is not pushed down
  }

  // motorB = map(joystick_steer_pos, 3000, 4095, 0, 180);
  // esc2.write(motorB);


  delay(50);
}


void setupMotors() {
  esc1.setPeriodHertz(60);
  esc2.setPeriodHertz(60);

  esc1.attach(ESC1_PIN, 500, 2500);
  esc2.attach(ESC2_PIN, 500, 2500);
}

void calibrateMotors() {
  Serial.println("Setting maximum throttle...");
  esc1.writeMicroseconds(2000);
  esc2.writeMicroseconds(2000);
  delay(MOTOR_CALIBRATION_DELAY);

  Serial.println("Setting minimum throttle...");
  esc1.writeMicroseconds(500);
  esc2.writeMicroseconds(500);
  delay(MOTOR_CALIBRATION_DELAY);
}
