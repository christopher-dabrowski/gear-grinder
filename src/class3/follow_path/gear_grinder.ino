#define ENGINE_BALANCING_DECREASE 4

void initRobot() {
  pinMode(D1, OUTPUT); // wł. i wył. prawego silnika
  pinMode(D2, OUTPUT); // wł. i wył. lewego silnika
  pinMode(D3, OUTPUT); // wybór kierunku prawego silnika
  pinMode(D4, OUTPUT); // wybór kierunku lewego silnika
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
