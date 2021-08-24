#include <Servo.h>
Servo R_servo;
const int DELAY = 1000;

void setup(){
  R_servo.attach(9);
  R_servo.write(90);
  delay(DELAY);
}
void loop(){
  R_servo.write(60);
  delay(DELAY);
  R_servo.write(90);
  delay(DELAY);
}
