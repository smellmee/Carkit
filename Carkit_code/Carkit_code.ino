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
decode_results old_value;

boolean aja_kokoajan = false;
boolean cont = false;
void setup(){
  Serial.begin(9600);
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
  speed = 0;
  dir = 1;
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  
  if (irrecv.decode(&results)) {
    
    if(results.value == OK.value)
    {
      if(speed == 255){
       speed = 0; 
      }else
      {
       speed = 255; 
      }
    }
    
    if(results.value == UP.value){old_value.value = results.value; dir = 1;}   
    if(results.value == DOWN.value){old_value.value = results.value; dir = 0;}
    if(results.value == LEFT.value) {old_value.value = results.value; dir = 3;}
    if(results.value == RIGHT.value) {old_value.value = results.value; dir = 2;}
    
    /*if(results.value == CONTINUOUS.value){
      if(old_value.value == UP.value){
      dir = 1;
      }
      if(old_value.value == DOWN.value){
      dir = 0;
      }
      if(old_value.value == LEFT.value){
      dir = 3;
      }
      if(old_value.value == RIGHT.value){
      dir = 2;
      }
    }*/
    
    /*if(aja_kokoajan == true){
      speed = 0;
    }else if(results.value == CONTINUOUS.value)
    {
      speed = 0;
      if(old_value.value == UP.value)
      {
        dir = 1;
      }
      
    }else{
     speed = 255; 
    }*/
   
    analogWrite(speedPinA,255 - speed);
    analogWrite(speedPinB,255 - speed);
    if(dir == 1)
    {
    digitalWrite(dir1PinA, LOW); //Kääntää eteenpäin
    digitalWrite(dir2PinA, HIGH); 
    digitalWrite(dir1PinB, HIGH);
    digitalWrite(dir2PinB, LOW);
    }else if(dir == 0)
    {
    digitalWrite(dir1PinA, HIGH); // Kääntää taaksepäin
    digitalWrite(dir2PinA, LOW);
    digitalWrite(dir1PinB, LOW);
    digitalWrite(dir2PinB, HIGH);
    }
    else if(dir == 3)
    {
    digitalWrite(dir1PinA, HIGH); // Kääntää vasempaan
    digitalWrite(dir2PinA, LOW);
    digitalWrite(dir1PinB, HIGH);
    digitalWrite(dir2PinB, LOW);
    }
    else if(dir == 2)
    {
    digitalWrite(dir1PinA, LOW); // Kääntää oikeaan
    digitalWrite(dir2PinA, HIGH);
    digitalWrite(dir1PinB, LOW);
    digitalWrite(dir2PinB, HIGH);
    }
    
    irrecv.resume(); // Receive the next value
  }
  
}





