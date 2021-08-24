const int DELAY1 = 500;//センサー読み込み

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  //センサの値を読み込む
  int L,M,R;
  L = analogRead(0);
  M = analogRead(1);
  R = analogRead(2);
  Serial.print(L);
  Serial.print(" ");
  Serial.print(M);
  Serial.print(" ");
  Serial.println(R);
  delay(DELAY1);
}
