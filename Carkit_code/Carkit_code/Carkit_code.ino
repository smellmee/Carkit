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

decode_results results, OK, UP, DOWN, RIGHT, LEFT, ONE, TWO, THREE, STAR, CONTINUOUS, STOP, old_value;

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
  STOP.value = 0;
  pinMode(dir1PinA, OUTPUT);
  pinMode(dir2PinA, OUTPUT);
  pinMode(speedPinA, OUTPUT);
  pinMode(dir1PinB, OUTPUT);
  pinMode(dir2PinB, OUTPUT);
  pinMode(speedPinB, OUTPUT);
  speed = 255;
  dir = 4;
  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  if (irrecv.decode(&results)) 
  {
/*    if(results.value == OK.value)
    {
      speed = 0;
    }*/
    
    if(results.value == UP.value){old_value.value = UP.value;}   
    if(results.value == DOWN.value){old_value.value = DOWN.value;}
    if(results.value == LEFT.value) {old_value.value = LEFT.value;}
    if(results.value == RIGHT.value) {old_value.value = RIGHT.value;}
       
    if(results.value == CONTINUOUS.value)
    { 
      Serial.println("CONTINUOUS");
      if(old_value.value == UP.value)
      {
        Serial.print("   UP");
        dir = 1;
        speed = 255;
      }
      else if(old_value.value == DOWN.value)
      {
        Serial.print("   DOWN");
        dir = 0;
        speed = 255;
      }
      else if(old_value.value == LEFT.value)
      {
        Serial.print("   LEFT");
        dir = 3;
        speed = 255;
      }  
      else if(old_value.value == RIGHT.value)
      {
        Serial.print("   RIGHT");
        dir = 2;
        speed = 255;
      }    
      else{
       Serial.println("  NA"); 
      }      
    }
    else{
    speed = 0;
    }
    
        
    analogWrite(speedPinA,255 - speed);
    analogWrite(speedPinB,255 - speed);
    
    if(dir == 1)
    {
    digitalWrite(dir1PinA, LOW); //Kääntää eteenpäin
    digitalWrite(dir2PinA, HIGH); 
    digitalWrite(dir1PinB, HIGH);
    digitalWrite(dir2PinB, LOW);
    irrecv.resume();
    }
    else if(dir == 0)
    {
    digitalWrite(dir1PinA, HIGH); // Kääntää taaksepäin
    digitalWrite(dir2PinA, LOW);
    digitalWrite(dir1PinB, LOW);
    digitalWrite(dir2PinB, HIGH);
    irrecv.resume();
    }
    else if(dir == 3)
    {
    digitalWrite(dir1PinA, HIGH); // Kääntää vasempaan
    digitalWrite(dir2PinA, LOW);
    digitalWrite(dir1PinB, HIGH);
    digitalWrite(dir2PinB, LOW);
    irrecv.resume();
    }
    else if(dir == 2)
    {
    digitalWrite(dir1PinA, LOW); // Kääntää oikeaan
    digitalWrite(dir2PinA, HIGH);
    digitalWrite(dir1PinB, LOW);
    digitalWrite(dir2PinB, HIGH);
    irrecv.resume();
    }
    else if(dir == 4){
     speed = 0;
     irrecv.resume();
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
