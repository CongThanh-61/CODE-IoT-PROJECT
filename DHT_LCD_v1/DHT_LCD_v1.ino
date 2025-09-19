#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Định nghĩa chân và loại cảm biến
#define DHTPIN 2      // Chân kết nối DATA của DHT22
#define DHTTYPE DHT22 // Loại cảm biến: DHT22

// Khởi tạo cảm biến và màn hình LCD
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Địa chỉ I2C của LCD có thể thay đổi (0x27 hoặc 0x3F)

void setup() {
  // Khởi động cảm biến DHT và LCD
  dht.begin();
  lcd.init();
  lcd.backlight(); // Bật đèn nền LCD
  
  // Hiển thị thông báo khởi động
  lcd.setCursor(0, 0);
  lcd.print("DHT22 Sensor");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Đọc dữ liệu từ cảm biến
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Kiểm tra lỗi đọc dữ liệu
  if (isnan(humidity) || isnan(temperature)) {
    lcd.setCursor(0, 0);
    lcd.print("Error reading");
    lcd.setCursor(0, 1);
    lcd.print("sensor data!");
    delay(2000);
    return;
  }

  // Hiển thị nhiệt độ và độ ẩm lên LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature, 1); // In nhiệt độ (1 số lẻ)
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Humid: ");
  lcd.print(humidity, 1); // In độ ẩm (1 số lẻ)
  lcd.print(" %");

  delay(2000); // Cập nhật mỗi 2 giây
}
