#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

// WiFi settings
const char *ssid = "GearGrinder";

// motor settings
#define RIGHT_MOTOR_PIN1 D1    // pin 1 of right motor (D2)
#define RIGHT_MOTOR_PIN2 D3    // pin 2 of right motor (D1)
#define LEFT_MOTOR_PIN1 D2     // pin 1 of left motor (D3)
#define LEFT_MOTOR_PIN2 D4     // pin 2 of left motor (D4)
#define RIGHT_MOTOR_SPEED 1023 // speed for right motor (0-1023)
#define LEFT_MOTOR_SPEED 1023  // speed for left motor (0-1023)
#define LED_PIN 2              // built-in LED (D4)

IPAddress local_ip(192, 168, 4, 1);  // IP for AP mode
IPAddress gateway(192, 168, 100, 1); // IP for AP mode
IPAddress subnet(255, 255, 255, 0);  // IP for AP mode
ESP8266WebServer server(80);

int car_mode = 0; // set car drive mode (0 = stop)

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
  WiFi.softAP(ssid);

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

// HTTP request: other
void handle_NotFound() {
  car_mode = 0;
  Serial.println("Page error");
  server.send(404, "text/plain", "Not found");
}

// control car movement
void car_control() {
  switch (car_mode) {
  case 0: // stop car
    digitalWrite(LEFT_MOTOR_PIN1, LOW);
    digitalWrite(RIGHT_MOTOR_PIN2, LOW);
    digitalWrite(RIGHT_MOTOR_PIN1, LOW);
    digitalWrite(LEFT_MOTOR_PIN2, LOW);
    break;
  case 1: // go forward
    digitalWrite(LEFT_MOTOR_PIN1, HIGH);
    digitalWrite(RIGHT_MOTOR_PIN2, HIGH);
    digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
    digitalWrite(LEFT_MOTOR_PIN2, HIGH);
    break;
  case 2: // go backward
    digitalWrite(LEFT_MOTOR_PIN1, HIGH);
    digitalWrite(RIGHT_MOTOR_PIN2, LOW);
    digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
    digitalWrite(LEFT_MOTOR_PIN2, LOW);
    break;
  case 3: // turn left
    digitalWrite(LEFT_MOTOR_PIN1, HIGH);
    digitalWrite(RIGHT_MOTOR_PIN2, HIGH);
    digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
    digitalWrite(LEFT_MOTOR_PIN2, LOW);
    break;
  case 4: // turn right
    digitalWrite(LEFT_MOTOR_PIN1, HIGH);
    digitalWrite(RIGHT_MOTOR_PIN2, LOW);
    digitalWrite(RIGHT_MOTOR_PIN1, HIGH);
    digitalWrite(LEFT_MOTOR_PIN2, HIGH);
  }
}

// output HTML web page for user
String SendHTML() {
  String html = "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";
  html += "<title>SIC Robot</title>";
  html += "<meta name=\"viewport\" content=\"width=device-width, "
          "initial-scale=1\">";
  html += "<style>body {background-color: PeachPuff;} h1 {color: DarkBlue;} h2 "
          "{color: DarkBlue;}</style>";
  html += "</head>";
  html += "<body>";
  html += "<div align=\"center\">";
  html += "<h1>GearGrinder is the best!</h1>";
  html += "<p>Michał & Krzysiek</p>";
  // html += "<h2>Robot WiFi</h2>";
  // html += "<h2>www.sic.edu.pl</h2>";
  // html += "<br>\n";
  html += "<form method=\"GET\">";
  html +=
      "<table><tr><td colspan=\"3\" align=\"center\"><input type=\"button\" "
      "style=\"height: 100px; width: 100px\" value=\"PROSTO\" "
      "onclick=\"window.location.href='/forward'\"></td></tr>";
  html += " <tr><td align=\"center\"><input type=\"button\" style=\"height: "
          "100px; width: 100px\" value=\"LEWO\" "
          "onclick=\"window.location.href='/left'\"></td><td "
          "align=\"center\"><input type=\"button\" style=\"height: 100px; "
          "width: 100px\" value=\"STOP\" "
          "onclick=\"window.location.href='/stop'\"></td><td "
          "align=\"center\"><input type=\"button\" style=\"height: 100px; "
          "width: 100px\" value=\"PRAWO\" "
          "onclick=\"window.location.href='/right'\"></td></tr>";
  html += "<tr><td colspan=\"3\" align=\"center\"><input type=\"button\" "
          "style=\"height: 100px; width: 100px\" value=\"WSTECZ\" "
          "onclick=\"window.location.href='/backward'\"></td></tr>";
  html += "</form>\n";
  html += "</div>\n";
  html += "</body>\n";
  html += "</html>\n";
  return html;
}
