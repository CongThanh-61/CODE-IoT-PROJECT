// Định nghĩa các chân cảm biến siêu âm  
const int trigPins[] = {A0, A2, A4, 10, 12};  // Trái, Trái trước, Giữa, Phải trước, Phải
const int echoPins[] = {A1, A3, A5, 9, 13};

// Định nghĩa chân cho động cơ
const int in1 = 4, in2 = 5, in3 = 6, in4 = 7, ENA = 3, ENB = 11;

// Định nghĩa chân cho công tắc hành trình
const int leftSwitch = 2, rightSwitch = 8;

// Các biến đo khoảng cách
long distances[5];

// Ngưỡng khoảng cách tránh vật cản
const int DIS = 30;
const int LeftSpeed = 110, RightSpeed = 110;

// Biến thời gian để quản lý delay không chặn
unsigned long previousMillis = 0;
const long interval = 100;

// Đọc khoảng cách từ cảm biến siêu âm
long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long pingTime = pulseIn(echoPin, HIGH, 30000);
  return (pingTime == 0) ? 999 : (pingTime / 2) * 0.0343;
}

void setup() {
  Serial.begin(9600);

  // Cấu hình chân cảm biến
  for (int i = 0; i < 5; i++) {
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }

  // Cấu hình động cơ
  pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  // Cấu hình công tắc hành trình
  pinMode(leftSwitch, INPUT_PULLUP);
  pinMode(rightSwitch, INPUT_PULLUP);

  moveStop();
}

void loop() {
  unsigned long currentMillis = millis();

  // Đọc trạng thái công tắc hành trình
  if (digitalRead(leftSwitch) == LOW) {
    handleCollision(true);
    return;
  }
  if (digitalRead(rightSwitch) == LOW) {
    handleCollision(false);
    return;
  }

  // Đọc tất cả các cảm biến siêu âm
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    for (int i = 0; i < 5; i++) {
      distances[i] = measureDistance(trigPins[i], echoPins[i]);
    }

    // Hiển thị dữ liệu cảm biến
    Serial.print("L: "); Serial.print(distances[0]);
    Serial.print(" LF: "); Serial.print(distances[1]);
    Serial.print(" M: "); Serial.print(distances[2]);
    Serial.print(" RF: "); Serial.print(distances[3]);
    Serial.print(" R: "); Serial.println(distances[4]);

    // Xử lý tránh vật cản
    processObstacle();
  }
}

void processObstacle() {
  long left = distances[0], leftFront = distances[1];
  long middle = distances[2], rightFront = distances[3], right = distances[4];

  if (middle < DIS) {
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

// Xử lý khi va chạm với công tắc hành trình
void handleCollision(bool isLeft) {
  moveStop();
  delay(200);
  moveBackward(600);
  isLeft ? turnRight(500) : turnLeft(500);
}

// Điều khiển động cơ
void moveForward() {
  Serial.println("Move Forward");
  analogWrite(ENA, LeftSpeed);
  analogWrite(ENB, RightSpeed);
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
}

void moveBackward(int time) {
  Serial.println("Move Backward");
  analogWrite(ENA, LeftSpeed);
  analogWrite(ENB, RightSpeed);
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
  delay(time);
}

void turnRight(int time) {
  Serial.println("Turn Right");
  analogWrite(ENA, LeftSpeed);
  analogWrite(ENB, RightSpeed);
  digitalWrite(in1, LOW); digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH); digitalWrite(in4, LOW);
  delay(time);
}

void turnLeft(int time) {
  Serial.println("Turn Left");
  analogWrite(ENA, LeftSpeed);
  analogWrite(ENB, RightSpeed);
  digitalWrite(in1, HIGH); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
  delay(time);
}

void moveStop() {
  Serial.println("Move Stop");
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(in1, LOW); digitalWrite(in2, LOW);
  digitalWrite(in3, LOW); digitalWrite(in4, LOW);
}