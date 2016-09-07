/*Written by Tatu Piippo on 1.9.2016*/

#include <IRremote.h>

//motor A
int dir1PinA = 2;
int dir2PinA = 3;
int speedPinA = 9;
//motor B
int dir1PinB = 4;
int dir2PinB = 5;
int speedPinB = 10;
int speed;
int dir;
int RECV_PIN = 11;
long unsigned int IRcode;

IRrecv irrecv(RECV_PIN);

decode_results results;
decode_results OK;
decode_results UP;
decode_results DOWN;
decode_results RIGHT;
decode_results LEFT;
decode_results ONE;
decode_results TWO;
decode_results THREE;
decode_results STAR;
decode_results CONTINUOUS;
void setup(){
  UP.value = 0xFF629D;
  OK.value = 0xFF02FD;
  DOWN.value = 0xFFA857;
  RIGHT.value = 0xFFC23D;
  LEFT.value = 0xFF22DD;
  ONE.value = 0xFF6897;
  TWO.value = 0xFF9867;
  THREE.value = 0xFFB04F;
  STAR.value = 0xFF42BD;
  CONTINUOUS.value = 0xFFFFFFFF;
  pinMode(dir1PinA, OUTPUT);
  pinMode(dir2PinA, OUTPUT);
  pinMode(speedPinA, OUTPUT);
  pinMode(dir1PinB, OUTPUT);
  pinMode(dir2PinB, OUTPUT);
  pinMode(speedPinB, OUTPUT);
  speed = 255;
  dir = 1;
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    
    if(results.value == OK.value)
    {
      if(speed == 255)
      {
       speed = 0; 
      }else
      {
       speed = 255; 
      }
    }
    if(results.value == UP.value){dir = 1;}   
    if(results.value == DOWN.value){dir = 0;}
    
    
    analogWrite(speedPinA,255 - speed);
    analogWrite(speedPinB,255 - speed);
    if(dir == 1)
    {
    digitalWrite(dir1PinA, LOW); //Kääntää eteenpäin
    digitalWrite(dir2PinA, HIGH); 
    digitalWrite(dir1PinB, HIGH);
    digitalWrite(dir2PinB, LOW);
    }else
    {
    digitalWrite(dir1PinA, HIGH); // Kääntää taaksepäin
    digitalWrite(dir2PinA, LOW);
    digitalWrite(dir1PinB, LOW);
    digitalWrite(dir2PinB, HIGH);
    }
    irrecv.resume(); // Receive the next value
  }
 
//    if (irrecv.decode(&results)) {
//    Serial.println(results.value, HEX);
//    irrecv.resume(); // Receive the next value
//  }
}






 /*analogWrite(speedPinA,speed); //Oikean puolen renkaat
  analogWrite(speedPinB,255 - speed); //Vasemman puolen renkaat
  // set direction
  if(1 == dir) {
    digitalWrite(dir1PinA, LOW);
    digitalWrite(dir2PinA, HIGH); //Kääntää eteenpäin
    digitalWrite(dir1PinB, HIGH);
    digitalWrite(dir2PinB, LOW);
  }else{
    digitalWrite(dir1PinA, HIGH); // Kääntää taaksepäin
    digitalWrite(dir2PinA, LOW);
    digitalWrite(dir1PinB, LOW);
    digitalWrite(dir2PinB, HIGH);
  }
  if(millis()-time>5000) {
    time = millis();
    speed += 20;
    if(speed > 255){
      speed = 0;
    }
    if(1==dir){
      dir = 0;
    }else{
      dir = 1;
    }
  }*/