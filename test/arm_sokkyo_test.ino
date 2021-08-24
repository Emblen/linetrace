#include <Servo.h>
Servo R_arm;
Servo L_arm;
Servo rot_arm;
double Duration = 0; //受信した間隔
double Distance = 0; //距離
const int akicanget = 10;//空き缶回収の時の距離
const int akicanthrow = 3;//空き缶を台に置くときの距離
#define echoPin 3 // Echo Pin
#define trigPin 2 // Trigger Pin

void setup() {
  Serial.begin(9600);
  R_arm.attach(9);
  L_arm.attach(10);
  rot_arm.attach(11);
  pinMode( echoPin, INPUT );
  pinMode( trigPin, OUTPUT );
  R_arm.write(130);
  L_arm.write(110);
  delay(300);
  rot_arm.write(50);
  delay(300);
}

void loop() {
  sokkyo();
  Serial.println(Distance);
  if (Distance < akicanget && Distance > akicanthrow){
    can_GET();
    if (Distance > 50){
       return 0;
    }
  }
  else if (Distance < akicanthrow){
    can_throw();
    while(true){
      return 0;
    }
  }
  else{
    return 0;
  }
  delay(200);
}

void sokkyo(){
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite( trigPin, HIGH ); //超音波を出力
  delayMicroseconds( 10 ); //
  digitalWrite( trigPin, LOW );
  Duration = pulseIn( echoPin, HIGH ); //センサからの入力
  if (Duration > 0) {
    Duration = Duration/2; //往復距離を半分にする
    Distance = Duration*340*100/1000000; // 音速を340m/sに設定
    return Distance;
  }
}

//空き缶を回収
void can_GET(){
  R_arm.write(160);
  L_arm.write(80);
  delay(500);
  rot_arm.write(140);
  delay(500);
}
//空き缶を台に置く
void can_throw(){
  rot_arm.write(50);
  delay(1000);
  R_arm.write(130);
  L_arm.write(110);
  delay(500);
}
