#define echoPin 3 // Echo Pin
#define trigPin 2 // Trigger Pin
 
double Duration = 0; //受信した間隔
double Distance = 0; //距離
void setup() {
Serial.begin( 9600 );
pinMode( echoPin, INPUT );
pinMode( trigPin, OUTPUT );
}
void loop() {
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite( trigPin, HIGH ); //超音波を出力
  delayMicroseconds( 10 ); //
  digitalWrite( trigPin, LOW );
  Duration = pulseIn( echoPin, HIGH ); //センサからの入力
  if (Duration > 0) {
    Duration = Duration/2; //往復距離を半分にする
    Distance = Duration*340*100/1000000; // 音速を340m/sに設定
    Serial.println(Distance);
  }
  delay(500);
}
