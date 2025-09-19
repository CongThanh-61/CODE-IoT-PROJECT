#define IR 3
#define relay 4
int giatriIR;

void setup()
{
  Serial.begin(9600);
  pinMode(IR,INPUT);
  pinMode(relay,OUTPUT);
}
void loop()
{
  giatriIR = digitalRead(IR);
  if(giatriIR == HIGH)
  {
    digitalWrite(relay,HIGH);
  }
  else
  {
    digitalWrite(relay,LOW);
  }
  delay(100);
}
