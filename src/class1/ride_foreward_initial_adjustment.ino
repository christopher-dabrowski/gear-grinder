void setup() {
  pinMode(D1, OUTPUT); // konfigurujemy pin wł i wył prawego silnika
  pinMode(D2, OUTPUT); // konfigurujemy pin wł i wył lewego silnika
  pinMode(D3, OUTPUT); // konfigurujemy pin wyboru kierunku prawego silnika
  pinMode(D4, OUTPUT); // konfigurujemy pin wyboru kierunku lewego silnika
}
void loop() {
  digitalWrite(D3, HIGH); // kierunek prawy do przodu
  digitalWrite(D4, HIGH); // kierunek lewy do przodu
  digitalWrite(D2, HIGH); // JAZDA - właczamy lewy silnik
  digitalWrite(D1, HIGH); // JAZDA - właczamy prawy silnik
  delay(1000);            // czekamy 1 sekundę
  digitalWrite(D3, LOW);  // kierunek prawy do tyłu
  digitalWrite(D4, LOW);  // kierunek lewy do tyłu
  digitalWrite(D2, LOW);  // STOP - wyłaczamy lewy silnik
  delay(50);              // czekamy 1 sekundę
  digitalWrite(D1, LOW);  // STOP - wyłaczamy prawy silnik
  delay(200);             // czekamy 1 sekundę
  digitalWrite(D3, LOW);  // kierunek prawy do tyłu
  digitalWrite(D4, LOW);  // kierunek lewy do tyłu
  digitalWrite(D1, HIGH); // JAZDA - właczamy prawy silnik
  digitalWrite(D2, HIGH); // JAZDA - właczamy lewy silnik
  delay(500);             // czekamy 1 sekundę
  digitalWrite(D3, LOW);  // kierunek prawy do tyłu
  digitalWrite(D4, LOW);  // kierunek lewy do tyłu
  digitalWrite(D1, LOW);  // STOP - wyłaczamy prawy silnik
  delay(50);              // czekamy 1 sekundę
  digitalWrite(D2, LOW);  // STOP - wyłaczamy lewy silnik
  delay(200);             // czekamy 1 sekundę
}
