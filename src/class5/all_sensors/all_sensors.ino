void setup() {
  initRobot();
  Serial.begin(9600);
}
void loop() {
  Serial.print("lewy=");
  Serial.print(sensorLeft());
  Serial.print("     prawy=");
  Serial.println(sensorRight());
  delay(100);

  // int pomiarOdległosci = sensorDistanceRead();
  // if (pomiarOdległosci > 250) {
  //   goFoward(20, 255);
  // } else {
  //   goBack(150, 255);
  //   turnRight(300, 255);
  // }
}
