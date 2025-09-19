ư#define BLYNK_TEMPLATE_ID "TMPL6X28DWlXk"
#define BLYNK_TEMPLATE_NAME "Relay Control"
#define BLYNK_AUTH_TOKEN "XFO3Uzh1vZ4xGB1eHsFC-LUvym-K1eRY"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h>
#include <EEPROM.h>

#define RELAY1_PIN  D1
#define RELAY2_PIN  D2
#define BUTTON1_PIN D5
#define BUTTON2_PIN D6
#define LED_WIFI    D7

bool relay1State = false;
bool relay2State = false;

#define EEPROM_SIZE 512
unsigned long buttonPressStart = 0;
bool buttonLongPressed = false;

#define LONG_PRESS_DURATION 10000  // 10 giây



void saveWiFiCredentials() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(0, WiFi.SSID());
  EEPROM.put(100, WiFi.psk());
  EEPROM.commit();
}

bool loadWiFiCredentials(String &ssid, String &pass) {
  char ssidBuff[64];
  char passBuff[64];
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(0, ssidBuff);
  EEPROM.get(100, passBuff);
  ssid = String(ssidBuff);
  pass = String(passBuff);
  return ssid.length() > 0;
}

void enterWiFiConfigMode() {
  digitalWrite(LED_WIFI, LOW);
  WiFiManager wm;
  wm.resetSettings(); // Xóa WiFi cũ
  if (!wm.autoConnect("ESP8266_Config", "12345678")) {
    Serial.println("⚠️ Không kết nối được!");
    ESP.restart();
  }

  saveWiFiCredentials();
  digitalWrite(LED_WIFI, HIGH);
}

void connectWiFi() {
  String ssid, pass;
  if (loadWiFiCredentials(ssid, pass)) {
    WiFi.begin(ssid.c_str(), pass.c_str());
    Serial.print("⏳ Đang kết nối WiFi: ");
    Serial.println(ssid);
    int tries = 0;
    while (WiFi.status() != WL_CONNECTED && tries++ < 20) {
      delay(500);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\n✅ Đã kết nối WiFi!");
      digitalWrite(LED_WIFI, HIGH);
      Blynk.begin(BLYNK_AUTH_TOKEN, ssid.c_str(), pass.c_str());
    } else {
      Serial.println("\n❌ Không kết nối được, vào cấu hình lại");
      enterWiFiConfigMode();
    }
  } else {
    enterWiFiConfigMode();
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(LED_WIFI, OUTPUT);

  digitalWrite(RELAY1_PIN, LOW);
  digitalWrite(RELAY2_PIN, LOW);
  digitalWrite(LED_WIFI, LOW);

  connectWiFi();
}

BLYNK_WRITE(V1) {
  relay1State = param.asInt();
  digitalWrite(RELAY1_PIN, relay1State ? HIGH : LOW);
}

BLYNK_WRITE(V2) {
  relay2State = param.asInt();
  digitalWrite(RELAY2_PIN, relay2State ? HIGH : LOW);
}

void loop() {
  Blynk.run();

  // ======= Xử lý nút BUTTON1 =======
  if (digitalRead(BUTTON1_PIN) == LOW) {
    if (buttonPressStart == 0) {
      buttonPressStart = millis();  // bắt đầu nhấn
    } else if (!buttonLongPressed && millis() - buttonPressStart > LONG_PRESS_DURATION) {
      Serial.println("🧠 Nhấn giữ >10s -> Vào chế độ cấu hình WiFi...");
      buttonLongPressed = true;
      enterWiFiConfigMode();  // Chuyển sang cấu hình WiFi
    }
  } else {
    if (buttonPressStart != 0 && !buttonLongPressed && millis() - buttonPressStart < 1000) {
      // Nhấn ngắn → chuyển relay
      relay1State = !relay1State;
      digitalWrite(RELAY1_PIN, relay1State ? HIGH : LOW);
      Blynk.virtualWrite(V1, relay1State);
      delay(200);
    }
    // Reset biến
    buttonPressStart = 0;
    buttonLongPressed = false;
  }

  // ======= Xử lý nút BUTTON2 =======
  if (digitalRead(BUTTON2_PIN) == LOW) {
    delay(200);
    relay2State = !relay2State;
    digitalWrite(RELAY2_PIN, relay2State ? HIGH : LOW);
    Blynk.virtualWrite(V2, relay2State);
    while (digitalRead(BUTTON2_PIN) == LOW);  // chờ nhả nút
  }
}
