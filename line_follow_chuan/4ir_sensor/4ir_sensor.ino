//Motor one
#define ENA 10 // ENA (L298) kết nối chân 10
#define IN1 9  // IN1 (L298) kết nối chân 9
#define IN2 8  // IN2 (L298) kết nối chân 8

//Motor two
#define IN3 7  // IN3 (L298) kết nối chân 7
#define IN4 6  // IN4 (L298) kết nối chân 6
#define ENB 5  // ENB (L298) kết nối chân 5

//Khai báo tốc độ động cơ
#define ENASpeed 100
#define ENBSpeed 100

void setup() {
  //Sensor pins
  pinMode(A0, INPUT); //cảm biến Trái ngoài kết nối với A0
  pinMode(A1, INPUT); //cảm biến trái trong kết nối A1
  pinMode(A2, INPUT); //cảm biến phải trong kết nối A2
  pinMode(A3, INPUT); //cảm biến phải ngoài kết nối A3
  //Motor one , khởi tạo chân ban đầu của 4 động cơ
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  //Motor two
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

//Khởi tạo chân xuất xung cho động cơ
  analogWrite(ENA,ENASpeed);
  analogWrite(ENB,ENBSpeed);
  delay(1000);

}
void loop() {
  //Đọc trạng thái cảm biến và đưa ra giá trị HIGH hoặc LOW
  bool SOne = digitalRead(A0);
  bool STwo = digitalRead(A1);
  bool SThree = digitalRead(A2);
  bool SFour = digitalRead(A3);
  //Nếu ir2 và ir3 trong line đen thì đi thẳng
  if (SOne == 0 && STwo == 1 && SThree == 1 && SFour == 0 ) {
    forward();
   //Nếu 4 cảm biến ở trong line đen thì dừng xe 
  } else if (SOne == 1 && STwo == 1 && SThree == 1 && SFour == 1) {
    Stop();
   //Nếu 4 cảm biến ở ngoài line trắng thì dừng xe 
  } else if (SOne == 0 && STwo == 0 && SThree == 0 && SFour == 0) {
    Stop();
   //Nếu ir1 và ir2 gặp line đen thì quẹo trái 
  }  else if (SOne == 1 && STwo == 1 && SThree == 0 && SFour == 0) {
    turnleft();
   // Nếu ir1 và ir2 và ir3 gặp line đen thì quẹo trái 
  }  else if (SOne == 1 && STwo == 1 && SThree == 1 && SFour == 0) {
    turnleft();
  // Nếu ir1  gặp line đen thì quẹo trái 
  }  else if (SOne == 1 && STwo == 0 && SThree == 0 && SFour == 0) {
    turnleft();
   //Nếu ir3 và ir4 gặp line đen thì quẹo phải 
  } else if (SOne == 0 && STwo == 0 && SThree == 1 && SFour == 1) {
    turnright();
  //Nếu ir2 và ir3 và ir4 gặp line đen thì quẹo phải  
  } else if (SOne == 0 && STwo == 1 && SThree == 1 && SFour == 1) {
    turnright();
  //Nếu ir4 gặp line đen thì quẹo phải 
  } else if (SOne == 0 && STwo == 0 && SThree == 0 && SFour == 1) {
    turnright();

  }
}

void forward() { 
//khai báo xuất tín hiệu cho động cơ đi thẳng
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void turnleft() {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void turnright() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void Stop() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}