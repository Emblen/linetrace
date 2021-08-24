#include <Servo.h>
Servo R_arm;
Servo L_arm;
Servo rot_arm;
const int threshold = 600;
const int PWM_r = 5;//右モーターPWMのpin番号
const int PWM_l = 6;//左モーターPWMのpin番号
const int rpin = 7;//右モーター出力pin番号
const int lpin = 8;//左モーター出力pin番号
const int DELAY = 30;//フォトリフレクタの動作間隔
const int Target = 250;//フォトリフレクタの目標値
const int stdPWM = 90;//モーターPWMの標準値
double Duration = 0; //受信した間隔
double Distance = 0; //距離
int akicanget = 10;//空き缶回収の時の距離(cm)
const int akicanthrow = 3;//空き缶を台に置くときの距離(cm)
#define echoPin 3 // Echo Pin
#define trigPin 2 // Trigger Pin
#define DELTA_T 0.03//フォトリフレクタの動作間隔と同じ
#define KP 0.1
#define KI 0
#define KD 0.002

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
  pinMode(echoPin,INPUT);
  pinMode(trigPin,OUTPUT);
  R_arm.attach(9);
  L_arm.attach(10);
  rot_arm.attach(11);
  R_arm.write(130);
  L_arm.write(110);
  delay(300);
  rot_arm.write(50);
}

void loop() {
  sokkyo();
  if(Distance<akicanget){
    if (Distance<10 && akicanget==10){
      akicanget=3;
      stoprunning();
      can_GET();
      //中央のセンサがライン上にくるまで左旋回
      //今中央のセンサはライン上にあるので、とりあえず１秒間左旋回する
      digitalWrite(PWM_r,HIGH);
      analogWrite(PWM_l,HIGH);
      digitalWrite(rpin,HIGH);
      digitalWrite(lpin,HIGH);
      delay(1000);
      while(true){
        int R = analogRead(2);
        if (R > threshold){
          analogWrite(PWM_r,HIGH);
          digitalWrite(PWM_l,HIGH);
          digitalWrite(rpin,HIGH);
          digitalWrite(lpin,HIGH);
          delay(200);
          break;
        }
        turn_right();
      }
    }
    else if(Distance<3 && akicanthrow==3){
      stoprunning();
      can_throw();
      delay(5000);
    }
  }
  //PID制御（センサ読み込み、バック、前進）
  PID();
}



//関数たち


//左に旋回
void turn_right(){
  digitalWrite(PWM_r,HIGH);
  analogWrite(PWM_l,HIGH);
  digitalWrite(rpin,HIGH);
  digitalWrite(lpin,HIGH);
  delay(DELAY);
}

//ストップ
void stoprunning(){
  digitalWrite(PWM_r,HIGH);
  digitalWrite(PWM_l,HIGH);
  digitalWrite(rpin,HIGH);
  digitalWrite(lpin,HIGH);
  delay(300);
}

//測距センサ
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
  delay(300);
  rot_arm.write(140);
  delay(800);
}
//空き缶を台に置く
void can_throw(){
  rot_arm.write(50);
  delay(800);
  R_arm.write(130);
  L_arm.write(110);
  delay(1000);
}

//PID制御
void PID(){
  //センサの値を読み込む
  int L,M,R;
  L = analogRead(0);
  M = analogRead(1);
  R = analogRead(2);

  //DCモーター制御
  if(L<threshold && M<threshold && R<threshold){
    //コースアウトしたらバックする
    analogWrite(PWM_r,250);
    analogWrite(PWM_l,200);
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
