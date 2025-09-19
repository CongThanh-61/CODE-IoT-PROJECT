#define BLYNK_TEMPLATE_ID "TMPL6ABKpLk3r"
#define BLYNK_TEMPLATE_NAME "WEATHER STATION MINI"
#define BLYNK_AUTH_TOKEN "91bMcnD-BpGcH87lPsVaUbm9W7uWkK0q"

#include <WiFi.h>
#include <Wire.h>
#include <BH1750.h>
#include <DHT.h>
#include <BlynkSimpleEsp32.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define RAIN_PIN 34 // analog pin

char ssid[] = "Virus";
char pass[] = "tan16108";

DHT dht(DHTPIN, DHTTYPE);
BH1750 lightMeter;
BlynkTimer timer;

void sendSensorData() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float lux = lightMeter.readLightLevel();
  int rainVal = analogRead(RAIN_PIN);

  // Kiểm tra giá trị đọc được
  if (isnan(h) || isnan(t)) {
    Serial.println("⚠️ DHT22 không phản hồi.");
    return;
  }

  Serial.print("🌡 Temp: "); Serial.print(t);
  Serial.print(" °C | 💧 Hum: "); Serial.print(h);
  Serial.print(" % | 🔆 Lux: "); Serial.print(lux);
  Serial.print(" | ☔ Rain: "); Serial.println(rainVal);

  // Gửi lên Blynk
  Blynk.virtualWrite(V0, (float)t);
  Blynk.virtualWrite(V1, (float)h);
  Blynk.virtualWrite(V2, (float)lux);
  Blynk.virtualWrite(V3, rainVal);  // mưa: int

}

void setup() {
  Serial.begin(115200);

  // Blynk + WiFi
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // DHT + BH1750
  dht.begin();
  Wire.begin(21, 22); // I2C cho ESP32
  lightMeter.begin();

  // Gửi data mỗi 5 giây
  timer.setInterval(5000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}
