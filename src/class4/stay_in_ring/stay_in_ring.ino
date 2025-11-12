#define SPEED 200
#define FORWARD_TIME 50

#define TURN_SPEED 255
#define TURN_TIME 150

#define WHITE LOW
#define BLACK HIGH

void setup() {
  initRobot();
  sensorRightLeftInit();
}
// bool done = false;
void loop() {
  int czujnikPrawy = sensorRight();
  int czujnikLewy = sensorLeft();

  // if (done) {
  //   delay(1000);
  //   return;
  // }

  // turnRight(800, 255);
  // done = true;
  // return;

  if (czujnikPrawy == BLACK || czujnikLewy == BLACK) {
    // try to turn back
    turnRight(800, 255);
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
