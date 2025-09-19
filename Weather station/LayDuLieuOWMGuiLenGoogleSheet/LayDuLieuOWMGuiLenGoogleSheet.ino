#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>  // Dùng thư viện Arduino_JSON

// ======= 1. Thông tin WiFi =======
const char* SSID     = "Bach";
const char* PASSWORD = "23101993";

// ======= 2. OpenWeatherMap API =======
const String OWM_API_KEY = "7fbf423feb05ca2ebdf818208fcf8ac8";
//const String CITY        = "Hanoi";               // Đặt tên thành phố mình muốn lấy
// Nếu muốn dùng lat/lon, thay bên dưới (và comment dòng CITY):
 const String lat = "10.97";
 const String lon = "106.65";

// ======= 3. Webhook Google Apps Script =======
const String WEBHOOK_URL = "https://script.google.com/macros/s/AKfycbwamnTIsXJ7LUGcM_WHC-1xpim3wiaiR-oz9r40dlve9fZZGMtlPK7V6LGlaNEWuW8xRQ/exec";

// ======= 4. Thời gian giữa 2 lần lấy/đẩy dữ liệu =======
unsigned long lastTime     = 0;
const unsigned long delayTime = 30000; // 10 phút = 600.000 ms

void setup() {
  Serial.begin(115200);
  delay(100);

  // Kết nối WiFi
  WiFi.begin(SSID, PASSWORD);
  Serial.print("🔌 Đang kết nối WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Kết nối WiFi thành công. IP: " + WiFi.localIP().toString());

  // Lần đầu fetch & send ngay khi khởi động
  fetchAndSendWeather();
}

void loop() {
  if (millis() - lastTime >= delayTime) {
    fetchAndSendWeather();
    lastTime = millis();
  }
}

// ========== Hàm lấy dữ liệu từ OpenWeatherMap và gửi lên Google ==========
void fetchAndSendWeather() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ WiFi chưa kết nối!");
    return;
  }

  // --- 1. Gọi API OpenWeatherMap ---
  // Nếu dùng theo city name:
 // String owmURL = "http://api.openweathermap.org/data/2.5/weather?q=" +
 //                 CITY + "&appid=" + OWM_API_KEY + "&units=metric";
  // Nếu dùng toạ độ lat/lon, uncomment 2 dòng sau và comment dòng trên:
   String owmURL = "http://api.openweathermap.org/data/2.5/weather?lat=" + lat + "&lon=" + lon + "&appid=" + OWM_API_KEY + "&units=metric";

  Serial.println("🌐 GET OWM URL: " + owmURL);
  HTTPClient http;
  http.begin(owmURL);
  int httpCode = http.GET();

  if (httpCode <= 0) {
    Serial.println("❌ Lỗi kết nối OWM, mã lỗi: " + String(httpCode));
    http.end();
    return;
  }

  String payload = http.getString();
  Serial.println("📦 JSON OWM nhận được: " + payload);
  http.end();

  // --- 2. Phân tích JSON ---
  JSONVar weatherObj = JSON.parse(payload);
  if (JSON.typeof(weatherObj) == "undefined") {
    Serial.println("❌ Lỗi phân tích JSON từ OWM");
    return;
  }

  // Ép kiểu rõ ràng từ JSONVar sang float
  float temp;
  {
    double temp_d = weatherObj["main"]["temp"];
    temp = (float) temp_d;
  }
  int humidity;
  {
    int hum_i = (int) weatherObj["main"]["humidity"];
    humidity = hum_i;
  }
  float windSpeed;
  {
    double wind_d = weatherObj["wind"]["speed"];
    windSpeed = (float) wind_d;
  }
  int pressure;
  {
    int p = (int)weatherObj["main"]["pressure"];
    pressure = p;
  }
  // Lấy mô tả thời tiết (string)
  String desc = (const char*) weatherObj["weather"][0]["main"];

  Serial.print("🌡️ Temp: "); Serial.print(temp); Serial.println(" °C");
  Serial.print("💧 Humidity: "); Serial.print(humidity); Serial.println(" %");
  Serial.print("💨 Wind Speed: "); Serial.print(windSpeed); Serial.println(" m/s");
  Serial.print("🔵 Pressure: "); Serial.print(pressure); Serial.println(" hPa");
  Serial.print("🌥️ Weather: "); Serial.println(desc);

  // --- 3. Gửi dữ liệu lên Google Apps Script (GET request) ---
  // Tạo URL kèm param: ?temp=...&humidity=...&wind=...&weather=...&city=...
  String postURL = WEBHOOK_URL +
                   "?temp=" + String(temp, 1) +
                   "&humidity=" + String(humidity) +
                   "&wind=" + String(windSpeed, 1) +
                   "&pressure=" + String(pressure) +
                   "&weather=" + desc +
                   "&sts=write";


  Serial.println("📤 Gửi lên Google: " + postURL);

  HTTPClient http2;
  http2.begin(postURL);
  int code2 = http2.GET();

  if (code2 > 0) {
    String resp = http2.getString();
    Serial.println("✅ Phản hồi từ Google Script: " + resp);
  } else {
    Serial.println("❌ Gửi lên Google lỗi. Mã: " + String(code2));
  }
  http2.end();
}
