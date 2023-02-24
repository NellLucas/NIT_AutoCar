#define UL_FRONT_TRIG 8
#define UL_FRONT_ECHO 9
#define UL_RIGHT_TRIG 12
#define UL_RIGHT_ECHO 13
#define UL_LEFT_TRIG 10
#define UL_LEFT_ECHO 11
#define IN1 0
#define IN2 1
#define IN3 2
#define IN4 3
#define ENA 7
#define ENB 4
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

int Duration_Front, Duration_Left, Duration_Right;
int Distance_Front, Distance_Left, Distance_Right;
int CASE, Front, Left, Right, value;

void setup() {
  Serial.begin(9600);
  sonic(UL_FRONT_TRIG, UL_FRONT_ECHO);
  sonic(UL_LEFT_TRIG, UL_LEFT_ECHO);
  sonic(UL_RIGHT_TRIG, UL_LEFT_ECHO);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  digitalWrite(UL_FRONT_TRIG, LOW);
  digitalWrite(UL_RIGHT_TRIG, LOW);
  digitalWrite(UL_LEFT_TRIG, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
}

void loop() {

   if(Serial.available())
   {
    int mid, spd;
    char middle[8], sped[6], cmd[10];
    //char cmd[11] = "210000100";    
    cmd[10] = Serial.read();
    memcpy(middle, &cmd[1],5);
    memcpy(sped, &cmd[6],3);
    middle[5], sped[3] = '\0';
    mid = atoi(middle);
    spd = atoi(sped);
    analogWrite(ENA, spd);
    analogWrite(ENB, spd);   
    
    if(cmd[0] == 1){
      if(mid>=-10000 && mid<0){
        Motor_Left();
      }
      else if(mid>=0 && mid<10000){
        Motor_Forward();
      }
      else if(mid>=10000 && mid>200000){
        Motor_Right();
      }
    }
    else if(cmd[0] == 2){
        
        Distance_Left = sonicdis(UL_LEFT_TRIG, UL_LEFT_ECHO);
        Distance_Right = sonicdis(UL_RIGHT_TRIG, UL_RIGHT_ECHO);
        Distance_Front = sonicdis(UL_FRONT_TRIG, UL_FRONT_ECHO);
        analogWrite(ENA, 255);
        analogWrite(ENB, 255);    
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
              case 0 : 
                Motor_Forward(); // 장애물 X
                break;
              case 1 : 
                Motor_Forward(); // 오른쪽
                break;
              case 2 : 
                Motor_Right(); // 왼쪽
                break;  
              case 3 : 
                Motor_Forward(); // 양 옆
                break;
              case 4 : 
                Motor_Right(); // 정면
                break;
              case 5 : 
                Motor_Left(); // 정면, 오른쪽
                break;
              case 6 : 
                Motor_Right(); // 정면, 왼쪽
                break;
              case 7 : 
                Motor_Turn(); // 180도
                break;
          }
        }
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
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(20);
}
void Motor_Turn(){
  Serial.println("180도 회전");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(20);
}
void Motor_Right(){
  Serial.println("우회전");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(20); 
}
void Motor_Forward(){
  Serial.println("전진");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(20);
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
