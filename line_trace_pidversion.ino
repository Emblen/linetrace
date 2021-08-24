const int threshold = 600;
const int PWM_r = 5;//右モーターPWMのpin番号
const int PWM_l = 6;//左モーターPWMのpin番号
const int rpin = 7;//右モーター出力pin番号
const int lpin = 8;//左モーター出力pin番号
const int DELAY = 30;//フォトリフレクタの動作間隔
const int Target = 350;//フォトリフレクタの目標値
const int stdPWM = 100;//モーターPWMの標準値
#define DELTA_T 0.03//フォトリフレクタの動作間隔と同じ
#define KP 0.15
#define KI 0.8
#define KD 0.001

//PID制御

//右車輪
float pid_R(int R){
  int diff_R[2];
  float integral_R;
  float p,i,d;

  diff_R[0] = diff_R[1];//前回の偏差の値
  diff_R[1] = R - Target;//ターゲットとの差
  integral_R += (diff_R[1] + diff_R[0])/2 * DELTA_T;//台形の面積

  p = KP * diff_R[1];
  i = KI * integral_R;
  d = KD * (diff_R[1] - diff_R[0]) / DELTA_T;

  int pidval_R = p+i+d;
  pidval_R = constrain(pidval_R,-255,255);//PWMの制限
  return pidval_R;
}

//左車輪
float pid_L(int L){
  int diff_L[2];
  float integral_L;
  float p,i,d;

  diff_L[0] = diff_L[1];//前回の偏差の値
  diff_L[1] = L - Target;//ターゲットとの差
  integral_L += (diff_L[1] + diff_L[0])/2 * DELTA_T;//台形の面積

  p = KP * diff_L[1];
  i = KI * integral_L;
  d = KD * (diff_L[1] - diff_L[0]) / DELTA_T;

  int pidval_L = p+i+d;
  pidval_L = constrain(pidval_L,-255,255);//PWMの制限
  return pidval_L;
}



void setup() {
  Serial.begin(9600);
  pinMode(PWM_r,OUTPUT);
  pinMode(PWM_l,OUTPUT);
  pinMode(rpin,OUTPUT);
  pinMode(lpin,OUTPUT);
}

void loop() {
  //センサの値を読み込む
  int L,M,R;
  L = analogRead(0);
  M = analogRead(1);
  R = analogRead(2);

  if(L<threshold && M<threshold && R<threshold){
    analogWrite(PWM_r,180);
    analogWrite(PWM_l,180);
    digitalWrite(rpin,LOW);
    digitalWrite(lpin,LOW);
    delay(300);
  }
  else{
    //PID制御
    int getPID_R = pid_R(R);
    int getPID_L = pid_L(L);
    //PWM値の決定
    int PWM_R = constrain((stdPWM + getPID_R - getPID_L),0,255);
    int PWM_L = constrain((stdPWM + getPID_L - getPID_R),0,255);
    
    Serial.print(L);
    Serial.print(" ");
    Serial.print(M);
    Serial.print(" ");
    Serial.print(R);
    Serial.print(" ");
    Serial.print(PWM_R);
    Serial.print(" ");
    Serial.println(PWM_L);
  
    //DCモーター制御
    analogWrite(PWM_r,PWM_R);
    analogWrite(PWM_l,PWM_L);
    digitalWrite(rpin,HIGH);
    digitalWrite(lpin,HIGH);
    delay(DELAY);
  }
  

}


//関数たち


//右に旋回
void turn_right(){
  analogWrite(PWM_l,200);
  digitalWrite(PWM_r,LOW);
  digitalWrite(PWM_l,HIGH);
  digitalWrite(rpin,HIGH);
  digitalWrite(lpin,HIGH);
  delay(DELAY);
}

//ストップ
void stoprunning(){
  analogWrite(PWM_r,HIGH);
  analogWrite(PWM_l,HIGH);
  digitalWrite(rpin,HIGH);
  digitalWrite(lpin,HIGH);
  delay(500);
}
