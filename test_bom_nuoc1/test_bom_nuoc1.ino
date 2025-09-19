// Dự án máy bơm nước uống tự động với Arduino Nano
// Thành phần:
// - Arduino Nano
// - Bơm DC 5V
// - Relay 5V
// - Cảm biến siêu âm HC-SR04

// Định nghĩa chân cho cảm biến siêu âm
const int trigPin = 5;
const int echoPin = 4;

// Định nghĩa chân điều khiển relay
const int relayPin = 6;

// Ngưỡng khoảng cách để kích hoạt bơm nước (đơn vị cm)
const int distanceThreshold = 5;

void setup() {
  // Cấu hình chân cho cảm biến siêu âm
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Cấu hình chân relay
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW); // Ban đầu tắt relay

  // Khởi tạo giao tiếp Serial để debug
  Serial.begin(9600);
}

void loop() {
  // Đo khoảng cách bằng cảm biến siêu âm
  long duration;
  int distance;

  // Gửi tín hiệu từ chân trig
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Đo thời gian tín hiệu phản hồi tại chân echo
  duration = pulseIn(echoPin, HIGH, 20000); // Thêm timeout 30ms để tránh treo

  if (duration == 0) {
    Serial.println("No echo received: Check sensor or connection");
    return; // Bỏ qua nếu không nhận được tín hiệu
  }

  // Tính toán khoảng cách
  distance = duration * 0.034 / 2;

  // Hiển thị khoảng cách để debug
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Kiểm tra khoảng cách và điều khiển bơm
  if (distance > 0 && distance < distanceThreshold) {
    digitalWrite(relayPin, HIGH); // Kích hoạt bơm
    Serial.println("Pump ON");
  } else {
    digitalWrite(relayPin, LOW); // Tắt bơm
    Serial.println("Pump OFF");
  }

  // Đợi 100ms trước khi lặp lại
  delay(100);
}
