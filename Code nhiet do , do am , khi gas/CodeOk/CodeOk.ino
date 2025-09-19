#define BLYNK_TEMPLATE_ID "TMPL63iD5DC3A"
#define BLYNK_TEMPLATE_NAME "DO NHIET DO DO AM VA CANH BAO KHI GAS"
#define BLYNK_AUTH_TOKEN "W1AprJ6ZVtevJbc7V3B_MUhdW_-F2T4c"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHTesp.h"

// LCD 16x2 địa chỉ 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHTesp dht;

char ssid[] = "Virus";
char pass[] = "tan16108";

BlynkTimer timer;
BlynkTimer blinkTimer;

int gas = 0;

#define BUTTON1_PIN 33
#define BUTTON2_PIN 32
#define LED1_PIN 26
#define LED2_PIN 25
#define DHT_PIN 4      // GPIO4 (D4)
#define MQ2_PIN 36     // GPIO36 (VP)

bool led1State = false;
bool led2State = false;

int lastButton1State = HIGH;
unsigned long lastDebounceTime1 = 0;
const unsigned long debounceDelay = 50;

int lastButton2State = HIGH;
unsigned long lastDebounceTime2 = 0;

bool isBlinking = false;
bool blinkState = false;
unsigned long blinkStartTime = 0;
const unsigned long blinkDuration = 5000;
int blinkTimerId = -1;

// Nhấp nháy đèn ảo trên Blynk (V0)
void blinkVirtualPin() {
  blinkState = !blinkState;
  Blynk.virtualWrite(V0, blinkState ? 1 : 0);

  if (millis() - blinkStartTime > blinkDuration) {
    blinkTimer.disable(blinkTimerId);
    Blynk.virtualWrite(V0, 1);
    isBlinking = false;
  }
}

// Gửi dữ liệu định kỳ lên Blynk
void sendSensorData() {
  int ketnoi1 = digitalRead(BUTTON1_PIN);
  int ketnoi2 = digitalRead(BUTTON2_PIN);
  float nhietdo = dht.getTemperature();
  float doam = dht.getHumidity();

  Blynk.virtualWrite(V0, isBlinking ? blinkState : 1);
  Blynk.virtualWrite(V1, String(nhietdo, 1));
  Blynk.virtualWrite(V2, gas);
  Blynk.virtualWrite(V3, led1State ? 1 : 0);
  Blynk.virtualWrite(V4, led2State ? 1 : 0);

}

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  dht.setup(DHT_PIN, DHTesp::DHT11);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, sendSensorData);

  // Đợi kết nối Blynk thành công
  while (!Blynk.connected()) {
    Blynk.run();
  }

  // Bắt đầu nhấp nháy đèn ảo V0 báo kết nối thành công
  isBlinking = true;
  blinkStartTime = millis();
  blinkTimerId = blinkTimer.setInterval(500L, blinkVirtualPin);
}

void loop() {
  Blynk.run();
  timer.run();
  blinkTimer.run();

  gas = analogRead(MQ2_PIN);

  // Xử lý debounce nút nhấn 1 (toggle LED1)
  int reading1 = digitalRead(BUTTON1_PIN);
  if (reading1 != lastButton1State) lastDebounceTime1 = millis();
  if ((millis() - lastDebounceTime1) > debounceDelay) {
    static int buttonState1 = HIGH;
    if (reading1 != buttonState1) {
      buttonState1 = reading1;
      if (buttonState1 == LOW) {
        led1State = !led1State;
        digitalWrite(LED1_PIN, led1State ? HIGH : LOW);
        Blynk.virtualWrite(V3, led1State ? 1 : 0);
      }
    }
  }
  lastButton1State = reading1;

  // Xử lý debounce nút nhấn 2 (toggle LED2)
  int reading2 = digitalRead(BUTTON2_PIN);
  if (reading2 != lastButton2State) lastDebounceTime2 = millis();
  if ((millis() - lastDebounceTime2) > debounceDelay) {
    static int buttonState2 = HIGH;
    if (reading2 != buttonState2) {
      buttonState2 = reading2;
      if (buttonState2 == LOW) {
        led2State = !led2State;
        digitalWrite(LED2_PIN, led2State ? HIGH : LOW);
        Blynk.virtualWrite(V4, led2State ? 1 : 0);
      }
    }
  }
  lastButton2State = reading2;

  // Hiển thị dữ liệu ra LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(dht.getTemperature(), 1);
  lcd.print("C ");

  lcd.print("H:");
  lcd.print(dht.getHumidity(), 0);
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Gas:");
  lcd.print(gas);

  // Cảnh báo bật LED1 nếu gas > 2500, ưu tiên bật cảnh báo
  if (gas > 2500) {
    digitalWrite(LED1_PIN, HIGH);
    led1State = true;
    Blynk.virtualWrite(V3, 1);
  } else {
    // Giữ trạng thái LED1 nếu bật qua nút nhấn hoặc nút ảo
    digitalWrite(LED1_PIN, led1State ? HIGH : LOW);
  }

  delay(200);
}

// Điều khiển LED1 qua nút ảo Blynk V3
BLYNK_WRITE(V3) {
  int pinValue = param.asInt();
  led1State = (pinValue == 1);
  digitalWrite(LED1_PIN, led1State ? HIGH : LOW);
}

// Điều khiển LED2 qua nút ảo Blynk V4
BLYNK_WRITE(V4) {
  int pinValue = param.asInt();
  led2State = (pinValue == 1);
  digitalWrite(LED2_PIN, led2State ? HIGH : LOW);
}
