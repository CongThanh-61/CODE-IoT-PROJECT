//Saad Innovative Ideas---Arduino Bluetooth Control Car. Copy this sketch and paste it in the

#define in1 3
#define in2 5
#define in3 6
#define in4 9
#define LEDF A1
#define LEDB 8
#define HORN 11
#define LEDP 10
//DBZ
int command; //Int to store app command state.
int Speed = 200; // 0 - 255.
int Speedsec;
int buttonState = 0;
int lastButtonState = 0;
int Turnradius = 0; //Set the radius of a turn, 0 - 255 Note:the

int brakeTime = 60;
int brkonoff = 1; //1 for the electronic braking system, 0 for

void setup() {
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);
pinMode(in3, OUTPUT);
pinMode(in4, OUTPUT);
pinMode(LEDF, OUTPUT); //Set the LED pin.
pinMode(LEDB, OUTPUT);
pinMode(HORN, OUTPUT);
pinMode(LEDP, OUTPUT);
Serial.begin(9600); //Set the baud rate to your Bluetooth module.
}
void loop() {
if (Serial.available() > 0) {
command = Serial.read();
Stop(); //Initialize with motors stoped.
switch (command) {
case 'F':
forward();
break;
case 'B':
back();
break;
case 'L':
left();
break;
case 'R':
right();
break;
case 'G':
forwardleft();
break;
case 'I':
forwardright();
break;
case 'H':
backleft();
break;
case 'J':
backright();
break;
case 'W':
frontledon();
break;
case 'w':
frontledoff();
break;
case 'U':
backledon();
break;
case 'u':
backledoff();
break;
case 'V':
hornon();
break;
case 'v':
hornoff();
break;
case 'X':
parkledon();
break;
case 'x':
parkledoff();
break;
case '0':
Speed = 100;
break;
case '1':
Speed = 140;
break;
case '2':
Speed = 153;
break;
case '3':
Speed = 165;
break;
case '4':
Speed = 178;
break;
case '5':
Speed = 191;
break;
case '6':
Speed = 204;
break;
case '7':
Speed = 216;
break;
case '8':
Speed = 229;
break;
case '9':
Speed = 242;
break;
case 'q':
Speed = 255;
break;
}
Speedsec = Turnradius;
if (brkonoff == 1) {
brakeOn();
} else {
brakeOff();
}
}
}
void right() {
analogWrite(in1, Speed);
analogWrite(in3, Speed);
}
void left() {
analogWrite(in2, Speed);
analogWrite(in4, Speed);
}
void forward() {
analogWrite(in3, Speed);
analogWrite(in2, Speed);
}
void back() {
analogWrite(in4, Speed);
analogWrite(in1, Speed);
}
void forwardleft() {
analogWrite(in1, Speedsec);
analogWrite(in3, Speed);
}
void forwardright() {
analogWrite(in1, Speed);
analogWrite(in3, Speedsec);
}
void backright() {
analogWrite(in2, Speed);
analogWrite(in4, Speedsec);
}
void backleft() {
analogWrite(in2, Speedsec);
analogWrite(in4, Speed);
}
void Stop() {
analogWrite(in1, 0);
analogWrite(in2, 0);
analogWrite(in3, 0);
analogWrite(in4, 0);
}
void frontledon() { //turn Front led on or off)
digitalWrite(A1, HIGH);
}
void frontledoff() { //turn Front led on or off)
digitalWrite(A1, LOW);
}
void backledon() { //turn Back led on or off)
digitalWrite(8, HIGH);
}
void backledoff() { //turn Back led on or off)
digitalWrite(8, LOW);
}
void hornon()
{
digitalWrite(7, HIGH);
}
void hornoff()
{
digitalWrite(7, LOW);
}
void parkledon()
{
digitalWrite(4,HIGH);
}
void parkledoff()
{
digitalWrite(4, LOW);
}
void brakeOn() {
buttonState = command;
if (buttonState != lastButtonState) {
if (buttonState == 'S') {
if (lastButtonState != buttonState) {
digitalWrite(in1, HIGH);
digitalWrite(in2, HIGH);
digitalWrite(in3, HIGH);
digitalWrite(in4, HIGH);
delay(brakeTime);
Stop();
}
}
lastButtonState = buttonState;
}
}
void brakeOff() {
}