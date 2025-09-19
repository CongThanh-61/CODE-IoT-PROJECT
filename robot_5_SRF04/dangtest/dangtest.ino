// Định nghĩa các chân cho 5 cảm biến siêu âm
int trig1 = A0;  // Trái
int echo1 = A1;
int trig2 = A2;  // Trái trước
int echo2 = A3;
int trig3 = A4;  // Giữa
int echo3 = A5;
int trig4 = 8;   // Phải trước
int echo4 = 9;
int trig5 = 12;  // Phải
int echo5 = 13;
int in1 = 4;
int in2 = 5;
int in3 = 6;
int in4 = 7;
int ENA = 3;
int ENB = 11;
// Các biến đo khoảng cách
long leftDistance = 0, leftFrontDistance = 0, middleDistance = 0, rightFrontDistance = 0, rightDistance = 0;

// Ngưỡng khoảng cách tránh vật cản
int DIS = 30;
long pingTime,distance;
float speedSound = 0.0343;
int LeftSpeed = 130;
int RightSpeed = 130;

// Hàm đo khoảng cách từ cảm biến
long measureDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long pingTime = pulseIn(echoPin, HIGH, 30000); // Timeout 30ms
  if (pingTime == 0) return 999;  // Không nhận tín hiệu
  return (pingTime / 2) * 0.0343; // Tính khoảng cách (cm)
}

void setup() {
  Serial.begin(9600);
  
  // Cấu hình các chân cảm biến
  pinMode(trig1, OUTPUT); pinMode(echo1, INPUT);
  pinMode(trig2, OUTPUT); pinMode(echo2, INPUT);
  pinMode(trig3, OUTPUT); pinMode(echo3, INPUT);
  pinMode(trig4, OUTPUT); pinMode(echo4, INPUT);
  pinMode(trig5, OUTPUT); pinMode(echo5, INPUT);
  
  // Cấu hình động cơ
  pinMode(in1, OUTPUT); pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);
  pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);

  moveStop();
}

void loop() {
  // Đọc khoảng cách từ các cảm biến
  leftDistance = measureDistance(trig1, echo1);
  delay(20);
  leftFrontDistance = measureDistance(trig2, echo2);
  delay(20);
  middleDistance = measureDistance(trig3, echo3);
  delay(20);
  rightFrontDistance = measureDistance(trig4, echo4);
  delay(20);
  rightDistance = measureDistance(trig5, echo5);
  delay(20);

  // Debug khoảng cách
  Serial.print("Left: "); Serial.print(leftDistance);
  Serial.print(", Left Front: "); Serial.print(leftFrontDistance);
  Serial.print(", Middle: "); Serial.print(middleDistance);
  Serial.print(", Right Front: "); Serial.print(rightFrontDistance);
  Serial.print(", Right: "); Serial.println(rightDistance);

  // Logic tránh vật cản
if (leftDistance < DIS && leftFrontDistance < DIS && middleDistance < DIS &&rightFrontDistance < DIS && rightDistance < DIS) {
  // Vật cản ở mọi hướng
  moveStop();
  delay(200);
  moveBackward();
  delay(600);
  turnRight();
  delay(1000); // Quay đầu 180 độ
} else if (middleDistance < DIS) {
  // Vật cản phía trước
  moveStop();
  delay(200);
  moveBackward();
  delay(600);
  if (leftFrontDistance > rightFrontDistance) {
    turnLeft();
    delay(500); // Quay trái
  } else {
    turnRight();
    delay(500); // Quay phải
  }
} else if (leftDistance < DIS || leftFrontDistance < DIS) {
  // Vật cản bên trái
  turnRight();
  delay(650);
} else if (rightDistance < DIS || rightFrontDistance < DIS) {
  // Vật cản bên phải
  turnLeft();
  delay(650);
} else if (leftDistance < DIS && rightDistance < DIS) {
  // Vật cản cả hai bên
  moveBackward();
  delay(500);
  turnLeft();
  delay(1000);
} else if (leftFrontDistance < DIS && middleDistance < DIS && rightFrontDistance < DIS) {
  // Vật cản cả ba phía trước
  moveStop();
  delay(200);
  moveBackward();
  delay(600);
  turnRight();
  delay(1000);
} else if (middleDistance < DIS && rightFrontDistance < DIS) {
  // Vật cản giữa và phải trước
  turnLeft();
  delay(500);
} else if (middleDistance < DIS && leftFrontDistance < DIS) {
  // Vật cản giữa và trái trước
  turnRight();
  delay(500);
} else {
  // Không có vật cản
  moveForward();
}

delay(300); // Tránh lặp quá nhanh
}
void moveForward()
{
  Serial.println("Move Forward");
  analogWrite(ENA,LeftSpeed);
  analogWrite(ENB,RightSpeed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW); 
}

void moveBackward()
{
  Serial.println("Move Backward");
  analogWrite(ENA,LeftSpeed);
  analogWrite(ENB,RightSpeed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH); 
}

void turnRight()
{
  Serial.println("Turn Right");
  analogWrite(ENA,LeftSpeed);
  analogWrite(ENB,RightSpeed);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnLeft()
{
  Serial.println("Turn Left");
  analogWrite(ENA,LeftSpeed);
  analogWrite(ENB,RightSpeed);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW); 
  digitalWrite(in3,LOW );
  digitalWrite(in4,HIGH);
}

void moveStop()
{
  Serial.println("Move Stop");
  analogWrite(ENA,LOW);
  analogWrite(ENB,LOW);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW); 
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW); 
}

