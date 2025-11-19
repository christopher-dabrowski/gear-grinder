#define SPEED 200
#define FORWARD_TIME 50

#define TURN_SPEED 255
#define TURN_TIME 150

#define WHITE LOW
#define BLACK HIGH

void setup() {
  initRobot();
  Serial.begin(9600);
}
// bool done = false;
void loop() {
  auto lineEncountered = handleLineDetection();
  if (lineEncountered)
    return;

  goFoward(FORWARD_TIME, SPEED);
}

bool handleLineDetection() {
  auto rightColor = sensorRight();
  auto leftColor = sensorLeft();

  if (rightColor == BLACK || leftColor == BLACK) {
    // TODO: Randomly choose how much to turn
    // try to turn back
    turnBack();
    return true;
  }
  if ((rightColor == WHITE) && (leftColor == BLACK)) {
    turnRight(TURN_TIME, TURN_SPEED);
    return true;
  }
  if ((rightColor == BLACK) && (leftColor == WHITE)) {
    turnLeft(TURN_TIME, TURN_SPEED);
    return true;
  }

  return false;
}
