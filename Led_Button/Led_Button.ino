void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Kích hoạt điện trở kéo lên
}

void loop() {
  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {         // Vì nút kéo xuống GND khi nhấn
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }
}