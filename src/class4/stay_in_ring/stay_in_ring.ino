#define SPEED 220
#define FORWARD_TIME 50

#define TURN_SPEED 255
#define TURN_TIME 150

#define WHITE LOW
#define BLACK HIGH

void setup() {
  initRobot();
  sensorRightLeftInit();
}
void loop() {
  int czujnikPrawy = sensorRight();
  int czujnikLewy = sensorLeft();

  if (czujnikPrawy == BLACK && czujnikLewy == BLACK) {
    // try to turn back
    turnRight(300, 255);
    return;
  }
  if ((czujnikPrawy == WHITE) && (czujnikLewy == WHITE)) {
    goFoward(FORWARD_TIME, SPEED);
    return;
  }

  if ((czujnikPrawy == WHITE) && (czujnikLewy == BLACK)) {
    turnRight(TURN_TIME, TURN_SPEED);
    return;
  }
  if ((czujnikPrawy == BLACK) && (czujnikLewy == WHITE)) {
    turnLeft(TURN_TIME, TURN_SPEED);
    return;
  }
}
