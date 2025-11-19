void setup() {
  initRobot();
  Serial.begin(9600);
}
void loop() {
  Serial.print("lewy=");
  Serial.print(sensorLeft());
  Serial.print("     prawy=");
  Serial.println(sensorRight());

  // Serial.println(hcsr04.ToString());

  Serial.print("dystans (mm)=");
  Serial.println(sensorDistanceRead());

  delay(500);

  // int pomiarOdległosci = sensorDistanceRead();
  // if (pomiarOdległosci > 250) {
  //   goFoward(20, 255);
  // } else {
  //   goBack(150, 255);
  //   turnRight(300, 255);
  // }
}
