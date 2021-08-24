const int PWM_r = 5;//右モーターPWM
const int PWM_l = 6;//左モーターPWM
const int rpin = 7;//右モーター出力
const int lpin = 8;//左モーター出力

void setup() {
  Serial.begin(9600);
  pinMode(rpin,OUTPUT);
  pinMode(lpin,OUTPUT);
  analogWrite(PWM_r,150);
  analogWrite(PWM_l,150);
}

void loop() {
  digitalWrite(rpin,HIGH);
  digitalWrite(lpin,HIGH);
  delay(2000);
  digitalWrite(rpin,LOW);
  digitalWrite(lpin,LOW);
  delay(2000);
}
