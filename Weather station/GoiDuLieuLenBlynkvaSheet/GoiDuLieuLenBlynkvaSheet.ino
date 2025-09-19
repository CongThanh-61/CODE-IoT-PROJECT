/******************************************************************
  Sketch này làm cùng lúc 3 việc:
  1. Đọc dữ liệu DHT22, BH1750, Rain Sensor → gửi lên Google Sheets
  2. Hiển thị dữ liệu local (nhiệt độ, độ ẩm, ánh sáng, mưa) lên Blynk
  3. Lấy dữ liệu OWM (nhiệt độ & độ ẩm) của Bình Dương → hiển thị lên Blynk
******************************************************************/
#define BLYNK_TEMPLATE_ID "TMPL6ABKpLk3r"
#define BLYNK_TEMPLATE_NAME "WEATHER STATION MINI"
#define BLYNK_AUTH_TOKEN "91bMcnD-BpGcH87lPsVaUbm9W7uWkK0q"
#include <time.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include <U8g2lib.h>
#include <DHT.h>          // DHT22
#include <Wire.h>
#include <BH1750.h>       // BH1750
#include <BlynkSimpleEsp32.h> // Blynk

/****************** 1. Cấu hình mạng  *************************/
const char*  WIFI_SSID     = "CAFE SOI";
const char*  WIFI_PASSWORD = "cafengon";

// Thông tin Google Apps Script Webhook (đã deploy)
const String WEBHOOK_URL = "https://script.google.com/macros/s/AKfycbyy7YFjC__CxBaZcc7j1pTikZfgFb1O0a3h3Re11DUffo9ursW08xpsdxAc8IV2WsmWVg/exec";

// Thông tin OpenWeatherMap
const String OWM_API_KEY = "7fbf423feb05ca2ebdf818208fcf8ac8";
const String lat = "10.97";
const String lon = "106.65";

// Thời gian giữa hai lần gửi Google Sheets (ms)
const unsigned long INTERVAL_UPLOAD = 10000; // 20 giây = 20.000 ms

/****************** 2. Khai báo chân & thư viện cảm biến ***************/

// --- DHT22: tín hiệu nối vào GPIO 19 (ví dụ bạn dùng pin này)
#define DHTPIN   19
#define DHTTYPE  DHT22
DHT dht(DHTPIN, DHTTYPE);

// --- BH1750: dùng I2C (GPIO 21 = SDA, GPIO 22 = SCL)
BH1750 lightMeter;

// --- Rain Sensor (Analog) nối vào GPIO 34 (ADC1_CH6)
#define RAIN_PIN 34

/******************** 3. Khởi tạo OLED U8g2 ***********************/

// Dùng driver SSD1306 128x64 (I2C).  
// Nếu bạn dùng SH1106 1.3" thì phải đổi thành U8G2_SH1106_xxx tương ứng.
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

/****************** 3. Biến lưu trữ & timer ***************************/
unsigned long lastUploadTime = 0;
bool getWeatherBD(float &tempBD, float &humiBD);
bool getHuggingFacePrediction(float temp, float humi, float light, int rainValue, String &result);
/****************** 4. Hàm khởi động ****************************************/
void setup() {
  Serial.begin(115200);
  delay(100);

  // Kết nối WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("🔌 Đang kết nối WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Đã kết nối WiFi! IP: " + WiFi.localIP().toString());

  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");  // GMT+7 cho VN
  // Khởi tạo DHT22
  dht.begin();
  // Khởi tạo BH1750
  Wire.begin(21, 22);
  lightMeter.begin();
  // Chân đọc mưa
  pinMode(RAIN_PIN, INPUT);

  // Khởi tạo Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);
   // 5.6. Khởi tạo OLED
  u8g2.begin();


  // Lần đầu tiên: lấy local + OWM + upload
  fetchAndSendAll();
}

/****************** 5. Hàm loop ***********************************************/
void loop() {
  Blynk.run();

  // Kiểm tra nếu đến thời điểm “upload” tiếp theo
  if (millis() - lastUploadTime >= INTERVAL_UPLOAD) {
    fetchAndSendAll();
    lastUploadTime = millis();
  }
}

/****************** 6. Hàm tổng hợp: Lấy data & Gửi lên Google & Blynk **********/
void fetchAndSendAll() {
  // 6.1 Đọc dữ liệu từ cảm biến local
  float temp_local  = dht.readTemperature();
  float humi_local  = dht.readHumidity();
  float light_lux   = lightMeter.readLightLevel();
  int   rainValue   = analogRead(RAIN_PIN);

  // Kiểm tra đọc DHT22 có hợp lệ không
  if (isnan(temp_local) || isnan(humi_local)) {
    Serial.println("❌ Lỗi đọc DHT22!");
    return;
  }
}
  Serial.println("=== DỮ LIỆU LOCAL ===");
  Serial.printf("Nhiệt độ local: %.1f °C\n", temp_local);
  Serial.printf("Độ ẩm local  : %.1f %%\n", humi_local);
  Serial.printf("Ánh sáng      : %.0f lux\n", light_lux);
  Serial.printf("Rain raw ADC  : %d\n", rainValue);

  // 6.2 Hiển thị lên Blynk (ví dụ: Virtual Pins: V0, V1, V2, V3)
  Blynk.virtualWrite(V0, temp_local);    // V0: Temp local
  Blynk.virtualWrite(V1, humi_local);    // V1: Humi local
  Blynk.virtualWrite(V2, light_lux);     // V2: Light
  Blynk.virtualWrite(V3, rainValue);     // V3: Rain
  
  String Hientai = "";

  if (temp_local > 30.0 && light_lux > 3000) {
    Hientai = "🌞 Nang gat";
  } 
  else if (temp_local <= 30 && light_lux < 3000) {
    Hientai = "⛅ Nhieu may";
  } 
  else if ( rainValue < 4095 ) {
    Hientai = "🌧️ Troi mua";
  } 
  else {
    Hientai = "🤔 Khong xac dinh";
  }

  // Gửi trạng thái thời tiết lên Blynk V6
  Serial.println("🌤️ Hientai: " + Hientai);
  Blynk.virtualWrite(V6, Hientai);


  // 6.3 Lấy dữ liệu OWM của Bình Dương (temp & humidity)
  float temp_BD = 0;
  float humi_BD = 0;
  if (getWeatherBD(temp_BD, humi_BD)) {
    Serial.println("=== DỮ LIỆU OWM Bình Dương ===");
    Serial.printf("Temp BD: %.1f °C\n", temp_BD);
    Serial.printf("Humi BD: %.1f %%\n", humi_BD);

    // Gửi lên Blynk: Virtual Pins V4, V5
    Blynk.virtualWrite(V4, temp_BD);  // V4: Temp BD
    Blynk.virtualWrite(V5, humi_BD);  // V5: Humi BD
  } else {
    Serial.println("❌ Lấy OWM Bình Dương thất bại");
  }

String aiPrediction = "";
if (getHuggingFacePrediction(temp_local, humi_local, light_lux, rainValue, aiPrediction)) {
  Blynk.virtualWrite(V7, aiPrediction);  // Gửi dự đoán lên Blynk V7
  Serial.println("🧠 Dự đoán AI: " + aiPrediction);
    // Hiển thị OLED (viết sau khi có dữ liệu)
  updateOLED(temp_local, humi_local, light_lux, rainValue, Hientai);

  // 6.4 Gửi dữ liệu đầy đủ lên Google Sheets
  sendToGoogleSheet(temp_local, humi_local, light_lux, rainValue);
}

/****************** 7. Hàm lấy dữ liệu OpenWeatherMap (Bình Dương) ***************/
bool getWeatherBD(float &tempBD, float &humiBD) {
  if (WiFi.status() != WL_CONNECTED) return false;

  String owmURL = "http://api.openweathermap.org/data/2.5/weather?lat=" + lat + "&lon=" + lon + "&appid=" + OWM_API_KEY + "&units=metric";


  HTTPClient http;
  http.begin(owmURL);
  int httpCode = http.GET();
  if (httpCode <= 0) {
    Serial.println("❌ OWM GET lỗi, code: " + String(httpCode));
    http.end();
    return false;
  }

  String payload = http.getString();
  http.end();
  // Parse JSON
  JSONVar obj = JSON.parse(payload);
  if (JSON.typeof(obj) == "undefined") {
    Serial.println("❌ Lỗi parse JSON OWM");
    return false;
  }
  // Ép kiểu rõ ràng
  {
    double d = obj["main"]["temp"];
    tempBD    = (float) d;
  }
  {
    int h = (int) obj["main"]["humidity"];
    humiBD    = (float) h;
  }
  return true;
}
String getCurrentTimeString() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "Thoi gian loi";
  }
  char buf[30];
  strftime(buf, sizeof(buf), "%d/%m %H:%M:%S", &timeinfo);
  return String(buf);
}

void updateOLED(float t_local, float h_local, float light_lux, int rainValue, String Hientai) {
  // Chọn font: ở đây dùng font nhỏ, dòng cao 8px
  u8g2.setFont(u8g2_font_5x8_tr	);
  String rainStatus;
  if (rainValue > 3500) {
    rainStatus = "Khong mua";
  } else if (rainValue > 1500) {
    rainStatus = "Mua nho";
  } else {
    rainStatus = "Mua to";
  }
  u8g2.firstPage();
  do {
    char buf[32];
    String currentTime = getCurrentTimeString();
    u8g2.drawStr(0, 8, currentTime.c_str());

    u8g2.drawStr(0, 16, "Tram Thoi Tiet Mini");

    snprintf(buf, sizeof(buf), "Nhiet do: %.1f C", t_local);
    u8g2.drawStr(0, 24, buf);

    snprintf(buf, sizeof(buf), "Do am   : %.1f %%", h_local);
    u8g2.drawStr(0, 32, buf);

    snprintf(buf, sizeof(buf), "Anh sang: %.0f lux", light_lux);
    u8g2.drawStr(0, 40, buf);

    snprintf(buf, sizeof(buf), "%s", rainStatus.c_str());
    u8g2.drawStr(0, 48, buf);

    snprintf(buf, sizeof(buf), "Trang thai: %s", Hientai.c_str());
    u8g2.drawStr(0, 56, buf);


  } while (u8g2.nextPage());
}

/****************** 8. Hàm gửi dữ liệu lên Google Sheets via Webhook *************/
void sendToGoogleSheet(float t_local, float h_local, float light_lux, int rainADC) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("❌ WiFi chưa kết nối!");
    return;
  }
  // Tạo URL GET kèm param
  String url = WEBHOOK_URL +
               "?sts=write" +
               "&temp_local=" + String(t_local, 1) +
               "&humi_local=" + String(h_local, 1) +
               "&light=" + String((int)light_lux) +
               "&rain=" + String(rainADC);


  Serial.println("📤 GỬI GOOGLE: " + url);
  HTTPClient http;
  http.begin(url);
  int code = http.GET();
  if (code > 0) {
    String resp = http.getString();
    Serial.println("✅ Phản hồi Google: " + resp);
  } else {
    Serial.println("❌ Gửi Google lỗi, code: " + String(code));
  }
  http.end();
}
// Hàm gọi Hugging Face model để dự đoán
bool getHuggingFacePrediction(float temp, float humi, float light, int rainValue, String &result) {
  if (WiFi.status() != WL_CONNECTED) return false;

  HTTPClient http;
  http.begin(HF_API_URL)
  http.addHeader("Content-Type", "application/json");

  String payload = "{\"inputs\": [" +
                   String(temp) + "," +
                   String(humi) + "," +
                   String((int)light) + "," +
                   String(rainValue) + "]}";

  int httpCode = http.POST(payload);
  if (httpCode > 0) {
    result = http.getString();
    Serial.println("🧠 HF Response: " + result);
    http.end();
    return true;
  } else {
    Serial.println("❌ HF API failed: " + String(httpCode));
    http.end();
    return false;
  }
}
