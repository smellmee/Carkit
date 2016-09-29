///Arduino Sample Code

#include <IRremote.h>

//motor A
int dir1PinA = 2;
int dir2PinA = 3;
//int speedPinA = A0;
//motor B
int dir1PinB = 4;
int dir2PinB = 5;
//int speedPinB = A1;
int generalSpeedPin = 10;
//Nopeus & Suunta
int speed;
int dir;
//Viivan seuraaja
int lineReaderLeftPin = 7;
int lineReaderMiddlePin = 8;
int lineReaderRightPin = 9;

short int leftSensorValue = 0;
short int middleSensorValue = 0;
short int rightSensorValue = 0;

short int leftSensorPreviousValue = 0;
short int middleSensorPreviousValue = 0;
short int rightSensorPreviousValue = 0;

short int lineToFollow = 1; //0 on valkoinen 1 on musta
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
  //pinMode(speedPinA, OUTPUT);
  pinMode(dir1PinB, OUTPUT);
  pinMode(dir2PinB, OUTPUT);
  pinMode(generalSpeedPin, OUTPUT);
  speed = 140;
  dir = 4;
  irrecv.enableIRIn(); // Start the receiver
  timeoutDelay = 100; 
}
void loop()
{
   if (irrecv.decode(&results)) 
  {
    if(results.value == STAR.value){
        if(followLine == true){
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
    
    /*if(leftSensorValue != leftSensorPreviousValue|| middleSensorValue != middleSensorPreviousValue|| rightSensorValue != rightSensorPreviousValue)
    { }*/
    /*Serial.print("Left: ");
    Serial.print(leftSensorValue); 
     Serial.print("  Mid: ");
    Serial.print(middleSensorValue);
     Serial.print("  right: ");
    Serial.println(rightSensorValue);*/
    
    
    //Käännä vaan toista takapuolta
    if(rightSensorValue == lineToFollow && leftSensorValue != lineToFollow && middleSensorValue != lineToFollow)
    {
     //Kääntyy vasempaan 
     digitalWrite(dir1PinA, HIGH); // Kääntää vasempaan
     digitalWrite(dir2PinA, LOW);
     digitalWrite(dir1PinB, HIGH);
     digitalWrite(dir2PinB, LOW);
     speed = 160;
    }else if(leftSensorValue == lineToFollow && rightSensorValue != lineToFollow && middleSensorValue != lineToFollow){
      //Kääntyy oikealle
      digitalWrite(dir1PinA, LOW); // Kääntää oikeaan
      digitalWrite(dir2PinA, HIGH);
      digitalWrite(dir1PinB, LOW);
      digitalWrite(dir2PinB, HIGH);
      speed = 160;
    }else if(middleSensorValue != lineToFollow && rightSensorValue != lineToFollow && leftSensorValue != lineToFollow){
     //Sensori on kadottanut valkoisen viivan kokonaan. Ajaa taaksepäin? 
     digitalWrite(dir1PinA, HIGH); // Kääntää taaksepäin
     digitalWrite(dir2PinA, LOW);
     digitalWrite(dir1PinB, LOW);
     digitalWrite(dir2PinB, HIGH);
     speed = 105;
    }else{
    //Joko kaikki sensorit näkee viivan tai pelkästään keskimmäinen sensori näkee viivan
    //Auto ajaa siis eteenpäin 
    digitalWrite(dir1PinA, LOW); //Kääntää eteenpäin
    digitalWrite(dir2PinA, HIGH); 
    digitalWrite(dir1PinB, HIGH);
    digitalWrite(dir2PinB, LOW);
    speed = 105;
    }
    analogWrite(generalSpeedPin, speed);
    delay(50);
    //analogWrite(speedPinB, speed);
  }/*else{
    analogWrite(generalSpeedPin, 0);
  }*/
  leftSensorPreviousValue = leftSensorValue;
  middleSensorPreviousValue = middleSensorValue;
  rightSensorPreviousValue = rightSensorValue;
}
