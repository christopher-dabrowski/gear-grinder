#include <hcsr04.h>

#define RIGHT_LINE_SENSOR_PIN D7
#define LEFT_LINE_SENSOR_PIN D8

#define TRIG_PIN D6
#define ECHO_PIN D5

#define ENGINE_BALANCING_DECREASE 15

HCSR04 hcsr04(TRIG_PIN, ECHO_PIN, 20, 4000);

int sensorDistanceRead() { return hcsr04.distanceInMillimeters(); }

void sensorRightLeftInit() {
  pinMode(RIGHT_LINE_SENSOR_PIN, INPUT);
  pinMode(LEFT_LINE_SENSOR_PIN, INPUT);
}

void initRobot() {
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  sensorRightLeftInit();
}

void goFoward(int czas, int predkosc) {
  digitalWrite(D3, HIGH);                                // jazda przód
  digitalWrite(D4, HIGH);                                // jazda przód
  analogWrite(D1, predkosc);                             // wolna jazda
  analogWrite(D2, predkosc - ENGINE_BALANCING_DECREASE); // wolna jazda
  delay(czas);
  stopEngine();
}
void goBack(int czas, int predkosc) {
  digitalWrite(D3, LOW);                                 // jazda tył
  digitalWrite(D4, LOW);                                 // jazda tył
  analogWrite(D1, predkosc);                             // wolna jazda
  analogWrite(D2, predkosc - ENGINE_BALANCING_DECREASE); // wolna jazda
  delay(czas);
  stopEngine();
}

void goRight(int czas, int predkosc) {
  digitalWrite(D3, HIGH);    // jazda przód
  digitalWrite(D4, HIGH);    // jazda przód
  analogWrite(D1, 0);        // stop
  analogWrite(D2, predkosc); // wolna jazda
  delay(czas);
  stopEngine();
}
void goLeft(int czas, int predkosc) {
  digitalWrite(D3, HIGH);    // jazda przód
  digitalWrite(D4, HIGH);    // jazda przód
  analogWrite(D1, predkosc); // wolna jazda
  analogWrite(D2, 0);        // stop
  delay(czas);
  stopEngine();
}

void turnRight(int czas, int predkosc) {
  digitalWrite(D3, LOW);     // jazda tył
  digitalWrite(D4, HIGH);    // jazda przód
  analogWrite(D1, predkosc); // wolna jazda
  analogWrite(D2, predkosc); // wolna jazda
  delay(czas);
  stopEngine();
}
void turnLeft(int czas, int predkosc) {
  digitalWrite(D3, HIGH);    // jazda przód
  digitalWrite(D4, LOW);     // jazda tył
  analogWrite(D1, predkosc); // wolna jazda
  analogWrite(D2, predkosc); // wolna jazda
  delay(czas);
  stopEngine();
}

void turnBack() { turnRight(800, 255); }

void stopEngine() {
  analogWrite(D1, 0); // stop
  analogWrite(D2, 0); // stop
}

void curveRight(int czas, int turn) {
  digitalWrite(D3, HIGH);      // jazda przód
  digitalWrite(D4, HIGH);      // jazda przód
  analogWrite(D1, 255 - turn); // mniejsza moc
  analogWrite(D2, 255);        // pełna moc
  delay(czas);
  stopEngine();
}
void curveLeft(int czas, int turn) {
  digitalWrite(D3, HIGH);      // jazda przód
  digitalWrite(D4, HIGH);      // jazda przód
  analogWrite(D1, 255);        // pełna moc
  analogWrite(D2, 255 - turn); // mniejsza moc
  delay(czas);
  stopEngine();
}

int sensorRight() { return digitalRead(RIGHT_LINE_SENSOR_PIN); }
int sensorLeft() { return digitalRead(LEFT_LINE_SENSOR_PIN); }
