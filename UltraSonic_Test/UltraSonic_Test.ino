#include <stdio.h> 

int echo1 = 13;
int echo2 = 11;
int trig1 = 12;
int trig2 = 10;

void setup() {
  Serial.begin(9600);
  seat(trig1, echo1);
  seat(trig2, echo2);
  pinMode(2, OUTPUT);
}

void loop() {
  analogWrite(2, 255);
  uss(echo1, trig1, 1);
  uss(echo2, trig2, 2);

}
void uss(int e, int t, int n){
  float cyc;
  float dis;
  digitalWrite(t, HIGH);
  delay(10);
  digitalWrite(t,LOW);

  cyc = pulseIn(e, HIGH);
  dis = ((340 * cyc) / 10000) / 2;  
  char buffer[10];
  sprintf(buffer, "%d Distance:", n);
  Serial.println(buffer);
  Serial.print(dis);
  Serial.println("cm");
  delay(100);
 
}
void seat(int t, int e){
  pinMode(t, OUTPUT);
  pinMode(e, INPUT);
}

void go(int pin){
  digitalWrite(pin, HIGH);
  digitalWrite(pin, LOW);
}
