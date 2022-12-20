#define UL_FRONT_TRIG 4
#define UL_FRONT_ECHO 5
#define UL_RIGHT_TRIG 6
#define UL_RIGHT_ECHO 7
#define UL_LEFT_TRIG 8
#define UL_LEFT_ECHO 9
#define MOTOR_LEFT_CW 10
#define MOTOR_LEFT_CCW 11
#define MOTOR_RIGHT_CW 12
#define MOTOR_RIGHT_CCW 13

int Duration_Front, Duration_Left, Duration_Right;
int Distance_Front, Distance_Left, Distance_Right;
int CASE, Front, Left, Right;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  sonic(UL_FRONT_TRIG, UL_FRONT_ECHO);
  sonic(UL_LEFT_TRIG, UL_LEFT_ECHO);
  sonic(UL_RIGHT_TRIG, UL_LEFT_ECHO);
  /*
  pinMode(MOTOR_LEFT_CW. OUTPUT);
  pinMode(MOTOR_RIGHT_CW, OUTPUT);
  pinMode(MOTOR_LEFT_CCW, OUTPUT);
  pinMode(MOTOR_RIGHT_CCW, OUTPUT);
  */
  digitalWrite(UL_FRONT_TRIG, LOW);
  digitalWrite(UL_RIGHT_TRIG, LOW);
  digitalWrite(UL_LEFT_TRIG, LOW);
  digitalWrite(MOTOR_LEFT_CW, LOW);
  digitalWrite(MOTOR_RIGHT_CW, LOW);
  digitalWrite(MOTOR_LEFT_CCW, LOW);
  digitalWrite(MOTOR_RIGHT_CCW, LOW);
}

void loop() {
  Distance_Left = sonicdis(UL_LEFT_TRIG, UL_LEFT_ECHO);
  Distance_Right = sonicdis(UL_RIGHT_TRIG, UL_RIGHT_ECHO);
  Distance_Front = sonicdis(UL_FRONT_TRIG, UL_FRONT_ECHO);


  dis_result();
  if(Distance_Left < 5){Left = 2;}
  else{Left = 0;}
  if(Distance_Right < 5){Right = 1;}
  else{Right = 0;}
  if(Distance_Front < 5){Front = 4;}
  else{Front = 0;}

CASE = Front + Left + Right;
switch (CASE)
{
  case 0 : Motor_Forward(); // 장애물 X
           break;
  case 1 : Motor_Forward(); // 오른쪽
           break;
  case 2 : Motor_Right(); // 왼쪽
           break;  
  case 3 : Motor_Forward(); // 양 옆
           break;
  case 4 : Motor_Right(); // 정면
           break;
  case 5 : Motor_Left(); // 정면, 오른쪽
           break;
  case 6 : Motor_Right(); // 정면, 왼쪽
           break;
  case 7 : Motor_Turn(); // 180도
           break;
  
}
}
void sonic(int t, int e){
  pinMode(t, OUTPUT);
  pinMode(e, INPUT);
}
long microsecondsToCentimeters(long microseconds){
  return microseconds/29/2;
}
void Motor_Left(){
  Serial.println("죄회전");
}
void Motor_Turn(){
  Serial.println("180도 회전");
}
void Motor_Right(){
  Serial.println("우회전");
}
void Motor_Forward(){
  Serial.println("전진");
}

void dis_result(void){
  char buffer[35];
  sprintf(buffer, "좌측:%d cm 정면:%d cm 우측:%d cm", Distance_Left, Distance_Front, Distance_Right);
  Serial.println(buffer);
}


int sonicdis(int tr, int ec){
  digitalWrite(tr, HIGH);
  delayMicroseconds(5);
  digitalWrite(tr, LOW);
  int Duration = pulseIn(ec, HIGH);
  int Distance = microsecondsToCentimeters(Duration);
  if(Distance < 0){Distance = ~Distance + 1;}
  return Distance;
}
