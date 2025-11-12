void setup() {
  initRobot();           // inicjalizacja robota
  sensorRightLeftInit(); // inicjalizacja czujników
  Serial.begin(9600);
}
void loop() {
  Serial.print("lewy=");
  Serial.print(sensorLeft());
  Serial.print("     prawy=");
  Serial.println(sensorRight());
  delay(100);
}
