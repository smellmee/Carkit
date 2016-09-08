/*Written by Tatu Piippo on 1.9.2016*/
#include <IRremote.h>
//Ultraäänisensori
int trigPin = 2;
int echoPin = 3;
int minDistance = 5;
//motor A
int dir1PinA = 2;
int dir2PinA = 3;
int speedPinA = 9;
//motor B
int dir1PinB = 4;
int dir2PinB = 5;
int speedPinB = 10;
//Nopeus ja suunta
int speed;
int dir;
//Kaukosäädin
int RECV_PIN = 11;
long unsigned int IRcode;
IRrecv irrecv(RECV_PIN);
decode_results results, OK, UP, DOWN, RIGHT, LEFT, ONE, TWO, THREE, STAR, CONTINUOUS;

void setup(){
  //Kaukosäädin
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
  //Moottorit
  pinMode(dir1PinA, OUTPUT);
  pinMode(dir2PinA, OUTPUT);
  pinMode(speedPinA, OUTPUT);
  pinMode(dir1PinB, OUTPUT);
  pinMode(dir2PinB, OUTPUT);
  pinMode(speedPinB, OUTPUT);
  speed = 255;
  dir = 1;
  irrecv.enableIRIn(); // Start the IR receiver
  //Ultraääni
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  if (irrecv.decode(&results)) {
    
    if(results.value == OK.value) {if(speed == 255){Stop();}
    else{Start();}}

    if(results.value == UP.value){dir = 1;}   
    if(results.value == DOWN.value){dir = 0;}
    if(results.value == RIGHT.value){/*Käännytään oikealle*/}
    if(results.value == LEFT.value){/*Käännytään vasemmalle*/}
    
    if(results.value == RIGHT.value && speed != 0){/*Käännytään oikealle sammuttamalla 1 oikean puolen moottori*/}
    if(results.value == LEFT.value && speed != 0){/*Käännytään vasemmalle sammuttamalla 1 vasemman puolen moottori*/}
    
    analogWrite(speedPinA,255 - speed);
    analogWrite(speedPinB,255 - speed);
    
    if(dir == 1)
    {
      digitalWrite(dir1PinA, LOW); //Kääntää eteenpäin
      digitalWrite(dir2PinA, HIGH); 
      digitalWrite(dir1PinB, HIGH);
      digitalWrite(dir2PinB, LOW);
    }
    else
    {
      digitalWrite(dir1PinA, HIGH); // Kääntää taaksepäin
      digitalWrite(dir2PinA, LOW);
      digitalWrite(dir1PinB, LOW);
      digitalWrite(dir2PinB, HIGH);
    }
    irrecv.resume(); // Receive the next value
  }
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;    
  if(distance <= minDistance){Stop();}
  delay(10);
}
void Stop()
{
  speed = 0;
}
void Start()
{
  speed = 255;
}
