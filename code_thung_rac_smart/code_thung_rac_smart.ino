#include <Servo.h>
Servo servoMain; // Define our Servo
int trigpin = 13;
int echopin = 12;
int distance;
float duration;
float cm;
void setup()
{
 servoMain.attach(11); // servo on digital pin 11
 pinMode(trigpin, OUTPUT);
 pinMode(echopin, INPUT);
}
void loop()
{
 digitalWrite(trigpin, LOW);
 delay(2);
 digitalWrite(trigpin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigpin, LOW);
 duration = pulseIn(echopin, HIGH);
 cm = (duration/58.82);
 distance = cm;

 if(distance<10)
 {
 servoMain.write(110); // Turn Servo back to center position (70 degrees)
 delay(2000);
 }
 else{
 servoMain.write(0);
 delay(50);
 }
}