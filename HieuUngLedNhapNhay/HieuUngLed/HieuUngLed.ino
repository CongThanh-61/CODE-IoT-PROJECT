
int led[10] = {2,3,4,5,6,7,8,9,10,11};
void setup()
{
  for(int i=0;i<=9;i++){
  pinMode(led[i] , OUTPUT);
  }
  
}

void loop()
{
 for(int j=0; j<3 ; j++)
 {
 for(int i=0; i<10;i++)
 {
   digitalWrite(led[i],HIGH);
 }
  delay(200);
 for(int i=0; i<10;i++)
 {
   digitalWrite(led[i],LOW);
   
 }
  delay(200);
 }
  for(int x=0;x<3;x++)
  {
  for(int i=0;i<=9;i++)
 {
    digitalWrite(led[i],HIGH);
    delay(100);
  }
  for(int i=9; i>=0; i--)
  {
    digitalWrite(led[i],LOW);
   delay(100);
  }
  }  // 5. Hiệu ứng chạy LED từ trái sang phải
  for (int i = 0; i < 10; i++) {
    digitalWrite(led[i], HIGH);
    delay(200);
    digitalWrite(led[i], LOW);
  }

  // 6. Hiệu ứng chạy LED từ phải sang trái
  for (int i = 9; i >= 0; i--) {
    digitalWrite(led[i], HIGH);
    delay(200);
    digitalWrite(led[i], LOW);
  }
  
}