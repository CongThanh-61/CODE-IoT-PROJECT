// Cấu hình chân cảm biến siêu âm  
const int trigPins[5] = {A0, A2, A4, 10, 12};  
const int echoPins[5] = {A1, A3, A5, 9, 13};

// Cấu hình chân động cơ
const int in1 = 4, in2 = 5, in3 = 6, in4 = 7, ENA = 3, ENB = 11;

// Cấu hình công tắc hành trình
const int leftSwitch = 2, rightSwitch = 8;

// Ngưỡng khoảng cách tránh vật cản
const int DIS = 30;
int LeftSpeed = 110, RightSpeed = 110;
long distances[5];
unsigned long previousMillis = 0;
const long interval = 100;  // Tần suất đọc cảm biến

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }
  pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
  pinMode(leftSwitch, INPUT_PULLUP);
  pinMode(rightSwitch, INPUT_PULLUP);
  moveStop();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    readSensors();
    processObstacle();
  }
}

void readSensors() {
  for (int i = 0; i < 5; i++) {
    distances[i] = measureDistance(trigPins[i], echoPins[i]);
  }
  Serial.print("L: "); Serial.print(distances[0]);
  Serial.print(" LF: "); Serial.print(distances[1]);
  Serial.print(" M: "); Serial.print(distances[2]);
  Serial.print(" RF: "); Serial.print(distances[3]);
  Serial.print(" R: "); Serial.println(distances[4]);
}

long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long pingTime = pulseIn(echoPin, HIGH, 30000);
  return (pingTime == 0) ? 999 : (pingTime / 2) * 0.0343;
}

void processObstacle() {
  long left = distances[0], leftFront = distances[1];
  long middle = distances[2], rightFront = distances[3], right = distances[4];

  if (left < DIS && leftFront < DIS && middle < DIS && rightFront < DIS && right < DIS) {
    moveBackward(600);
    turnRight(1000);
  } else if (middle < DIS) {
    moveBackward(600);
    (leftFront > rightFront) ? turnLeft(500) : turnRight(500);
  } else if (left < DIS || leftFront < DIS) {
    turnRight(500);
  } else if (right < DIS || rightFront < DIS) {
    turnLeft(500);
  } else {
    moveForward();
  }
}

void moveForward() {
  analogWrite(ENA, LeftSpeed);
  analogWrite(ENB, RightSpeed);
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
}

void moveBackward(int time) {
  analogWrite(ENA, LeftSpeed);
  analogWrite(ENB, RightSpeed);
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
  delay(time);
}

void turnRight(int time) {
  analogWrite(ENA, LeftSpeed);
  analogWrite(ENB, RightSpeed);
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
  delay(time);
}

void turnLeft(int time) {
  analogWrite(ENA, LeftSpeed);
  analogWrite(ENB, RightSpeed);
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
  delay(time);
}

void moveStop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW);
}
