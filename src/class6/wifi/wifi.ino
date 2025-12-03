#include "secrets.h"
#include "website.h"
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <pgmspace.h>

// WiFi settings
const char *ssid = "GearGrinder";

// motor settings
#define RIGHT_MOTOR_PIN1 D1          // pin 1 of right motor (D2)
#define RIGHT_MOTOR_PIN2 D3          // pin 2 of right motor (D1)
#define LEFT_MOTOR_PIN1 D2           // pin 1 of left motor (D3)
#define LEFT_MOTOR_PIN2 D4           // pin 2 of left motor (D4)
#define RIGHT_MOTOR_SPEED 1023       // speed for right motor (0-1023)
#define LEFT_MOTOR_SPEED 1023        // speed for left motor (0-1023)
#define ENGINE_BALANCING_DECREASE 15 // compensate for motor differences
#define LED_PIN 2                    // built-in LED (D4)

IPAddress local_ip(192, 168, 4, 1);  // IP for AP mode
IPAddress gateway(192, 168, 100, 1); // IP for AP mode
IPAddress subnet(255, 255, 255, 0);  // IP for AP mode
ESP8266WebServer server(80);

int car_mode = 0;        // set car drive mode (0 = stop)
int leftMotorSpeed = 0;  // left motor speed (-1023 to 1023)
int rightMotorSpeed = 0; // right motor speed (-1023 to 1023)

// initialize
void setup() {

  Serial.begin(115200);
  Serial.println("SIC_Robot");
  pinMode(RIGHT_MOTOR_PIN1, OUTPUT);
  pinMode(RIGHT_MOTOR_PIN2, OUTPUT);
  pinMode(LEFT_MOTOR_PIN1, OUTPUT);
  pinMode(LEFT_MOTOR_PIN2, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  car_control(); // stop the car

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, WIFI_PASSWORD);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // setup web server to handle specific HTTP requests
  server.on("/", HTTP_GET, handle_OnConnect);
  server.on("/forward", HTTP_GET, handle_forward);
  server.on("/backward", HTTP_GET, handle_backward);
  server.on("/left", HTTP_GET, handle_left);
  server.on("/right", HTTP_GET, handle_right);
  server.on("/stop", HTTP_GET, handle_stop);
  server.on("/motor", HTTP_GET, handle_motor);
  server.onNotFound(handle_NotFound);

  // start server
  server.begin();
  Serial.println("ESP8266 car server started.");
  digitalWrite(LED_PIN, LOW);
}

// handle HTTP requests and control car
void loop() {
  server.handleClient();
  car_control();
}

// HTTP request: on connect
void handle_OnConnect() {
  car_mode = 0;
  Serial.println("Client connected");
  server.send(200, "text/html", SendHTML());
}

// HTTP request: stop car
void handle_stop() {
  car_mode = 0;
  Serial.println("Stopped");
  server.send(200, "text/html", SendHTML());
}

// HTTP request: go forward
void handle_forward() {
  car_mode = 1;
  Serial.println("Go forward...");
  server.send(200, "text/html", SendHTML());
}

// HTTP request: go backward
void handle_backward() {
  car_mode = 2;
  Serial.println("Go backward...");
  server.send(200, "text/html", SendHTML());
}

// HTTP request: turn left
void handle_left() {
  car_mode = 3;
  Serial.println("Turn left...");
  server.send(200, "text/html", SendHTML());
}

// HTTP request: turn right
void handle_right() {
  car_mode = 4;
  Serial.println("Turn right...");
  server.send(200, "text/html", SendHTML());
}

// HTTP request: motor control with speed
void handle_motor() {
  if (server.hasArg("left") && server.hasArg("right")) {
    leftMotorSpeed = server.arg("left").toInt();
    rightMotorSpeed = server.arg("right").toInt();
    // Clamp values to valid range
    leftMotorSpeed = constrain(leftMotorSpeed, -1023, 1023);
    rightMotorSpeed = constrain(rightMotorSpeed, -1023, 1023);
  }
  server.send(200, "text/plain", "OK");
}

// HTTP request: other
void handle_NotFound() {
  car_mode = 0;
  Serial.println("Page error");
  server.send(404, "text/plain", "Not found");
}

// control car movement using PWM differential drive
void car_control() {
  // Set direction pins based on speed sign
  digitalWrite(RIGHT_MOTOR_PIN2, rightMotorSpeed >= 0 ? HIGH : LOW);
  digitalWrite(LEFT_MOTOR_PIN2, leftMotorSpeed >= 0 ? HIGH : LOW);

  // Set PWM speed (apply engine balancing to left motor)
  int leftPWM = max(0, abs(leftMotorSpeed) - ENGINE_BALANCING_DECREASE);
  int rightPWM = abs(rightMotorSpeed);

  analogWrite(RIGHT_MOTOR_PIN1, rightPWM);
  analogWrite(LEFT_MOTOR_PIN1, leftPWM);
}

// Return a single String instance constructed once from the PROGMEM HTML
String SendHTML() {
  static String html;
  if (html.length() == 0) {
    html = FPSTR(index_html);
  }
  return html;
}
