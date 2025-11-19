#define FOREWORD_SPEED 200
#define FORWARD_TIME 50

#define TURN_SPEED 255
#define TURN_TIME 150

void setup() {
  initRobot();
  Serial.begin(9600);
}
// bool done = false;
void loop() {
  auto lineEncountered = handleLineDetection();
  if (lineEncountered)
    return;

  auto distance = sensorDistanceRead();
  Serial.print("Distance=");
  Serial.println(distance);

  if (distance < 200) {
    Serial.println("Enemy detected! Full speed ahead!");
    goFoward(400, FOREWORD_SPEED);
    return;
  }

  Serial.println("Searching for enemy...");
  goFoward(100, FOREWORD_SPEED);
  turnRight(200, 255);
}

#define WHITE LOW
#define BLACK HIGH

bool handleLineDetection() {
  auto rightColor = sensorRight();
  auto leftColor = sensorLeft();

  Serial.print("lewy=");
  Serial.print(leftColor);
  Serial.print("     prawy=");
  Serial.println(rightColor);

  if (rightColor == BLACK || leftColor == BLACK) {
    // TODO: Randomly choose how much to turn
    // try to turn back
    Serial.println("Line detected! Turning back.");
    turnBack();
    return true;
  }
  // if ((rightColor == WHITE) && (leftColor == BLACK)) {
  //   turnRight(TURN_TIME, TURN_SPEED);
  //   return true;
  // }
  // if ((rightColor == BLACK) && (leftColor == WHITE)) {
  //   turnLeft(TURN_TIME, TURN_SPEED);
  //   return true;
  // }

  return false;
}
