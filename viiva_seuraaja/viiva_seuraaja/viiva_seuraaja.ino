///Arduino Sample Code

#include <IRremote.h>
//motor A
int dir1PinA = 2;
int dir2PinA = 3;
int speedPinA = 9;
//motor B
int dir1PinB = 4;
int dir2PinB = 5;
int speedPinB = 10;
//Nopeus & Suunta
int speed;
int dir;
//Viivan seuraaja
int lineReaderLeftPin = 1;
int lineReaderMiddlePin = 7;
int lineReaderRightPin = 8;

short int leftSensorValue = 0;
short int middleSensorValue = 0;
short int rightSensorValue = 0;

//Kaukosäädin
int RECV_PIN = 11;
long unsigned int IRcode, gap, lastPress, timeoutDelay;
IRrecv irrecv(RECV_PIN);
decode_results results, OK, UP, DOWN, RIGHT, LEFT, ONE, TWO, THREE, STAR, CONTINUOUS, STOP, old_value;

boolean followLine = false;

void setup()
{
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
  speed = 25;
  dir = 4;
  irrecv.enableIRIn(); // Start the receiver
  timeoutDelay = 100; 
}
void loop()
{
   if (irrecv.decode(&results)) 
  {
    if(results.value == STAR.value){
        if(followLine){
          followLine = false;
      }else{
       followLine = true; 
      }
     }
  }
  if(followLine){
    leftSensorValue = digitalRead(lineReaderLeftPin); // Arvo on 0 kun valkoista ja 1 jos mustaa
    middleSensorValue = digitalRead(lineReaderMiddlePin);
    rightSensorValue = digitalRead(lineReaderRightPin);
    
    
    Serial.print("Left: ");
    Serial.print(leftSensorValue); 
     Serial.print("  Mid: ");
    Serial.print(middleSensorValue);
     Serial.print("  right: ");
    Serial.println(rightSensorValue);
    
    if(leftSensorValue == 0 && rightSensorValue != 0)
    {
     //Kääntyy vasempaan 
     digitalWrite(dir1PinA, HIGH); // Kääntää vasempaan
     digitalWrite(dir2PinA, LOW);
     digitalWrite(dir1PinB, HIGH);
     digitalWrite(dir2PinB, LOW);
    }else if(rightSensorValue == 0 && leftSensorValue != 0){
      //Kääntyy oikealle
      digitalWrite(dir1PinA, LOW); // Kääntää oikeaan
      digitalWrite(dir2PinA, HIGH);
      digitalWrite(dir1PinB, LOW);
      digitalWrite(dir2PinB, HIGH);
    }else if(middleSensorValue != 0 && rightSensorValue != 0 && leftSensorValue != 0){
     //Sensori on kadottanut valkoisen viivan kokonaan. Ajaa taaksepäin? 
     digitalWrite(dir1PinA, HIGH); // Kääntää taaksepäin
     digitalWrite(dir2PinA, LOW);
     digitalWrite(dir1PinB, LOW);
     digitalWrite(dir2PinB, HIGH);
    }else{
    //Joko kaikki sensorit näkee viivan tai pelkästään keskimmäinen sensori näkee viivan
    //Auto ajaa siis eteenpäin 
    digitalWrite(dir1PinA, LOW); //Kääntää eteenpäin
    digitalWrite(dir2PinA, HIGH); 
    digitalWrite(dir1PinB, HIGH);
    digitalWrite(dir2PinB, LOW);
    }
  }
  
}
