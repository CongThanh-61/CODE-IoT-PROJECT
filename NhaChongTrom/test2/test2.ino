#define pir1 4
#define pir2 5 
#define led 6
#define coi 7
#define button1 8
#define button2 9
#define button3 10

bool chedo1 = false;  
bool chedo2 = false;
bool chedo3 = false;

bool nut1DaNhan = false;
bool nut2DaNhan = false;
bool nut3DaNhan = false;

unsigned long lastPress1 = 0;
unsigned long lastPress2 = 0;
unsigned long lastPress3 = 0;
const int debounceDelay = 200;

void setup() {
  pinMode(pir1, INPUT);
  pinMode(pir2, INPUT);  
  pinMode(led, OUTPUT);
  pinMode(coi, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  int giatriPIR1 = digitalRead(pir1);
  int giatriPIR2 = digitalRead(pir2);
  int giatributton1 = digitalRead(button1);
  int giatributton2 = digitalRead(button2);
  int giatributton3 = digitalRead(button3);
  unsigned long currentMillis = millis();

  // Xử lý nút nhấn chế độ 3
  if (giatributton3 == LOW && !nut3DaNhan && (currentMillis - lastPress3 > debounceDelay)) {
    chedo3 = !chedo3;
    nut3DaNhan = true;
    lastPress3 = currentMillis;
    Serial.print("che do 3: ");
    Serial.println(chedo3);
  }
  if (giatributton3 == HIGH) nut3DaNhan = false;

  if (chedo3) {
    digitalWrite(coi, (giatriPIR1 == HIGH || giatriPIR2 == HIGH) ? HIGH : LOW);
  }

  // Xử lý nút nhấn chế độ 2
  if (giatributton2 == LOW && !nut2DaNhan && (currentMillis - lastPress2 > debounceDelay)) {
    chedo2 = !chedo2;
    nut2DaNhan = true;
    lastPress2 = currentMillis;
    Serial.print("che do 2: ");
    Serial.println(chedo2);
  }
  if (giatributton2 == HIGH) nut2DaNhan = false;

  // Nếu chế độ 2 bật, tắt toàn bộ hệ thống
  if (chedo2) {
    digitalWrite(led, LOW);
    digitalWrite(coi, LOW);
    return;
  }

  // Xử lý nút nhấn chế độ 1
  if (giatributton1 == LOW && !nut1DaNhan && (currentMillis - lastPress1 > debounceDelay)) {
    chedo1 = !chedo1;
    nut1DaNhan = true;
    lastPress1 = currentMillis;
    Serial.print("che do 1: ");
    Serial.println(chedo1);
  }
  if (giatributton1 == HIGH) nut1DaNhan = false;

  if (chedo1) {
    digitalWrite(led, (giatriPIR1 == HIGH || giatriPIR2 == HIGH) ? HIGH : LOW);
  } else {
    // Chế độ mặc định: bật cả LED và còi khi có chuyển động
    if (giatriPIR1 == HIGH || giatriPIR2 == HIGH) {
      digitalWrite(led, HIGH);
      digitalWrite(coi, HIGH);
    } else {
      digitalWrite(led, LOW);
      digitalWrite(coi, LOW);
    }
  }
}