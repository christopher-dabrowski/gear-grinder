#define SPEED = 200;
#define FORWARD_TIME = 50;

#define TURN_SPEED = 255;
#define TURN_TIME = 50;

void setup() {
  initRobot();
  sensorRightLeftInit();
}
void loop() {
  int czujnikPrawy = sensorRight();
  int czujnikLewy = sensorLeft();
  if ((czujnikPrawy == LOW) && (czujnikLewy == LOW)) {
    goFoward(FORWARD_TIME, SPEED);
  } else {
    if ((czujnikPrawy == HIGH) && (czujnikLewy == LOW)) {
      turnRight(TURN_TIME, TURN_SPEED);
      goFoward(FORWARD_TIME, SPEED);
    } else {
      if ((czujnikPrawy == LOW) && (czujnikLewy == HIGH)) {
        turnLeft(TURN_TIME, TURN_SPEED);
        goFoward(FORWARD_TIME, SPEED);
      } else {
        delay(500);
      }
    }
  }
}
