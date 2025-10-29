#define SINGLE_RIDE_TIME_MS 500
#define LED_BUILTIN_1 2
#define LED_BUILTIN_2 16

void setup() {
  pinMode(LED_BUILTIN_1, OUTPUT);
  pinMode(LED_BUILTIN_2, OUTPUT);

  pinMode(D1, OUTPUT); // konfigurujemy pin wł i wył prawego silnika
  pinMode(D2, OUTPUT); // konfigurujemy pin wł i wył lewego silnika
  pinMode(D3, OUTPUT); // konfigurujemy pin wyboru kierunku prawego silnika
  pinMode(D4, OUTPUT); // konfigurujemy pin wyboru kierunku lewego silnika
}

void displayLedValue(int value) {
  if (value == 0) {
    digitalWrite(LED_BUILTIN_1, HIGH);
    digitalWrite(LED_BUILTIN_2, HIGH);
  } else if (value == 1) {
    digitalWrite(LED_BUILTIN_1, HIGH);
    digitalWrite(LED_BUILTIN_2, LOW);
  } else if (value == 2) {
    digitalWrite(LED_BUILTIN_1, LOW);
    digitalWrite(LED_BUILTIN_2, HIGH);
  } else {
    digitalWrite(LED_BUILTIN_1, LOW);
    digitalWrite(LED_BUILTIN_2, LOW);
  }
}

void runBothEngines(int runTimeMs) {
  digitalWrite(D2, HIGH); // JAZDA - właczamy lewy silnik
  digitalWrite(D1, HIGH); // JAZDA - właczamy prawy silnik

  delay(runTimeMs);
  digitalWrite(D1, LOW); // STOP - wyłaczamy prawy silnik
  digitalWrite(D2, LOW); // STOP - wyłaczamy lewy silnik
}

void rideForward(int rightDelayMs = 50) {
  digitalWrite(D3, HIGH); // kierunek prawy do przodu
  digitalWrite(D4, HIGH); // kierunek lewy do przodu
  digitalWrite(D2, HIGH); // JAZDA - właczamy lewy silnik
  digitalWrite(D1, HIGH); // JAZDA - właczamy prawy silnik

  delay(SINGLE_RIDE_TIME_MS);
  digitalWrite(D2, LOW); // STOP - wyłaczamy lewy silnik
  // The robot sways to the right so the right wheel needs to work a bit longer
  // in each cycle
  delay(rightDelayMs);
  digitalWrite(D1, LOW); // STOP - wyłaczamy prawy silnik
}

void rideBackward(int leftDelayMs = 50) {
  digitalWrite(D3, LOW);  // kierunek prawy do tyłu
  digitalWrite(D4, LOW);  // kierunek lewy do tyłu
  digitalWrite(D2, HIGH); // JAZDA - właczamy lewy silnik
  digitalWrite(D1, HIGH); // JAZDA - właczamy prawy silnik

  delay(SINGLE_RIDE_TIME_MS);
  digitalWrite(D1, LOW); // STOP - wyłaczamy prawy silnik
  // When going backward, the robot sways to the left, so the left wheel needs
  // to work a bit longer
  delay(leftDelayMs);
  digitalWrite(D2, LOW); // STOP - wyłaczamy lewy silnik
}

void rotateClockwise(int rotateTime) {
  digitalWrite(D3, HIGH); // kierunek prawy do przodu
  digitalWrite(D4, LOW);  // kierunek lewy do tyłu
  digitalWrite(D2, HIGH); // JAZDA - właczamy lewy silnik
  digitalWrite(D1, HIGH); // JAZDA - właczamy prawy silnik

  delay(rotateTime);
  digitalWrite(D1, LOW); // STOP - wyłaczamy prawy silnik
  digitalWrite(D2, LOW); // STOP - wyłaczamy lewy silnik
}

void rotateCounterClockwise(int rotateTime) {
  digitalWrite(D3, LOW);  // kierunek prawy do tyłu
  digitalWrite(D4, HIGH); // kierunek lewy do przodu
  digitalWrite(D2, HIGH); // JAZDA - właczamy lewy silnik
  digitalWrite(D1, HIGH); // JAZDA - właczamy prawy silnik

  delay(rotateTime);
  digitalWrite(D1, LOW); // STOP - wyłaczamy prawy silnik
  digitalWrite(D2, LOW); // STOP - wyłaczamy lewy silnik
}

bool done = false;

void loop() {
  // if (done) {
  //   delay(1000);
  //   return;
  // }

  for (int i = 0; i < 5; i++) {
    rideForward(75);
  }

  rotateClockwise(380);

  for (int i = 0; i < 5; i++) {
    rideForward(75);
  }

  rotateClockwise(380);

  // done = true;

  // int delaysToTest[] = {0, 50, 75, 100};

  // for (int i = 0; i < sizeof(delaysToTest) / sizeof(delaysToTest[0]); i++) {
  //   int delayMs = delaysToTest[i];
  //   displayLedValue(i);
  //   for (int i = 0; i < 3; i++) {
  //     rideForward(delayMs);
  //   }
  //   delay(3000);
  // }
}
