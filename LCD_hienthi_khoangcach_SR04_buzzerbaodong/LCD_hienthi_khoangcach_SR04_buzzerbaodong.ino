#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TRIG_PIN 10
#define ECHO_PIN 11
#define BUZZER_PIN 12  // Pin để điều khiển còi báo động

LiquidCrystal_I2C lcd(0x27, 16, 2); // Địa chỉ I2C của màn hình LCD 16x2

void setup() {
  // Khởi tạo LCD
  lcd.begin(16, 2);

  lcd.backlight();
  // Khởi tạo các pin cảm biến siêu âm và còi báo động
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT); // Cài đặt pin còi báo động là OUTPUT
}

void loop() {
  // Gửi xung Trigger để đo khoảng cách
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Đo thời gian Echo trở lại
  long duration = pulseIn(ECHO_PIN, HIGH);
  
  // Tính toán khoảng cách (tốc độ âm thanh = 343 m/s)
  long distance = (duration * 0.0344) / 2;
  
  // Hiển thị khoảng cách lên LCD
  lcd.setCursor(0, 0);  // Đặt con trỏ tại dòng thứ 2
  lcd.print("Khoang cach: ");
  lcd.print(distance);
  lcd.print(" cm");
  
  // Kiểm tra khoảng cách và kích hoạt còi báo động nếu khoảng cách < 20cm
  if (distance < 30) {
    digitalWrite(BUZZER_PIN, HIGH);  // Bật còi báo động
    delay(5000);
  } else {
    digitalWrite(BUZZER_PIN, LOW);   // Tắt còi báo động
     delay(5000);
  }
   if (distance < 40) {
    digitalWrite(BUZZER_PIN, HIGH);  // Bật còi báo động
    delay(3000);
  } else {
    digitalWrite(BUZZER_PIN, LOW);   // Tắt còi báo động
     delay(2000);
       if (distance < 20) {
    digitalWrite(BUZZER_PIN, HIGH);  // Bật còi báo động
    delay(2000);
  } else {
    digitalWrite(BUZZER_PIN, LOW);   // Tắt còi báo động
     delay(500);
  }
  }
  delay(500);  // Đợi một chút trước khi đo lại
}
