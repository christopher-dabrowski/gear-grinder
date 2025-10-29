#define LED_BUILTIN_1 2
#define LED_BUILTIN_2 16

void setup() {
  pinMode(LED_BUILTIN_1, OUTPUT);
  pinMode(LED_BUILTIN_2, OUTPUT);
}
void loop() {
  digitalWrite(LED_BUILTIN_1, HIGH);
  digitalWrite(LED_BUILTIN_2, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN_1, LOW);
  digitalWrite(LED_BUILTIN_2, LOW);
  delay(1000);
}
