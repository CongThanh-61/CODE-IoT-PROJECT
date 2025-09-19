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

int giatriPIR1 = 0;
int giatriPIR2 = 0;
int giatributton1 = 0;
int giatributton2 = 0;
int giatributton3 = 0;
void setup() 
{
  pinMode(pir1, INPUT);
  pinMode(pir2, INPUT);  
  pinMode(led, OUTPUT);
  pinMode(coi, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3,INPUT_PULLUP);
  
  Serial.begin(9600);
}

void loop()
{
 giatriPIR1 = digitalRead(pir1);
 giatriPIR2 = digitalRead(pir2);
 giatributton1 = digitalRead(button1);
 giatributton2 = digitalRead(button2);
 giatributton3 = digitalRead(button3);
  //Xử lí nút nhấn chế độ 3
  if(giatributton3 == LOW && !nut3DaNhan)
  {
    delay(200);
    if(digitalRead(button3) == LOW)
    {
      chedo3 = !chedo3;
      nut3DaNhan = true;
      Serial.print("che do 3: ");
      Serial.println(chedo3);
    }
  }
    if(giatributton3 == HIGH)
    {
      nut3DaNhan = false;
    }
    if(chedo3)
    {
     if(giatriPIR1 == HIGH || giatriPIR2 == HIGH)
  {
    digitalWrite(coi,HIGH);
  }
  else
  {
    digitalWrite(coi,LOW);
  }
    }
   // Xử lý nút nhấn chế độ 2
  if (giatributton2 == LOW && !nut2DaNhan) 
  {
    delay(200);
    if (digitalRead(button2) == LOW) 
    {
      chedo2 = !chedo2;
      nut2DaNhan = true;
      Serial.print("che do 2: ");
      Serial.println(chedo2);
    }
  }
  if (giatributton2 == HIGH) 
  {
  nut2DaNhan = false;
  }
  // Nếu chế độ 2 bật, tắt toàn bộ hệ thống
  if (chedo2)
   {
    digitalWrite(led, LOW);
    digitalWrite(coi, LOW);
//  return;  // Thoát loop để không xử lý tiếp
  }

  // Xử lý nút nhấn chế độ 1
  if (giatributton1 == LOW && !nut1DaNhan) 
  {
    delay(200);
    if (digitalRead(button1) == LOW) 
    {
      chedo1 = !chedo1;
      nut1DaNhan = true;
      Serial.print("che do 1: ");
      Serial.println(chedo1);
    }
  }
  if (giatributton1 == HIGH) 
  {
  nut1DaNhan = false;
  }
  
  if (chedo1) 
  {
    if (giatriPIR1 == HIGH || giatriPIR2 == HIGH) 
  {
    digitalWrite(led, HIGH);
  } 
  else 
  {
    digitalWrite(led, LOW);
  }
  } 
   else 
   {
    // Chế độ mặc định: bật cả LED và còi khi có chuyển động
    if (giatriPIR1 == HIGH || giatriPIR2 == HIGH) 
    {
      digitalWrite(led, HIGH);
      digitalWrite(coi, HIGH);
    } else 
    {
      digitalWrite(led, LOW);
      digitalWrite(coi, LOW);
    }
  }
}



