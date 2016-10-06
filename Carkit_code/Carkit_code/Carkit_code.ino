#include <Servo.h>

/*T.V. Tatu Piippo 9v*/
#include <IRremote.h>
//motor A
int dir1PinA = 2;
int dir2PinA = 3;
//int speedPinA = 9;
//motor B
int dir1PinB = 4;
int dir2PinB = 5;
//int speedPinB = 10;
int generalSpeedPin = 10;

//Nopeus & Suunta
int speed;
int dir;
//Kaukosäädin
int RECV_PIN = 11;
long unsigned int IRcode, gap, lastPress, timeoutDelay;
IRrecv irrecv(RECV_PIN);
//harpake
Servo radarServo;
int servoPin = 6;
int servopos = 90;
int servoDir = 0;
int servoStep = 10;
boolean radar = false;
boolean STOPFORTHELOVEOFGOD = false;
boolean spinServo = false;
//Ultraäänisensori
int trigPin = 13;
int echoPin = 12;
unsigned long timestamp = 0;
long duration, distance;
int crashTreshold = 15;
decode_results results, OK, UP, DOWN, RIGHT, LEFT, ONE, TWO, THREE, STAR, CONTINUOUS, STOP, old_value;

//Viivan seuraaja
int lineReaderLeftPin = 7;
int lineReaderMiddlePin = 8;
int lineReaderRightPin = 9;

short int leftSensorValue = 0;
short int middleSensorValue = 0;
short int rightSensorValue = 0;

short int lineToFollow = 1; //0 on valkoinen 1 on musta
boolean followLine = false;

void setup(){
  Serial.begin(9600);
  Serial.print("AT+NAMESanik"); //Nimeää bluetooth laitteen
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
  //pinMode(speedPinB, OUTPUT);
  pinMode(servoPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(generalSpeedPin, OUTPUT);
  speed = 255;
  dir = 4;
  irrecv.enableIRIn(); // Start the receiver
  timeoutDelay = 100;
  radarServo.attach(servoPin);
}

void loop() {
  
  if(Serial.available())
  {//if there is data being recieved
    dir = Serial.read(); //read it
    speed = 255;
    driveTo(); //Ajaa dir mukaiseen suuntaa
  }
  
  if(radar)
  {
    doRadarThings();
  }
  
  if (irrecv.decode(&results)) 
  {
    lastPress = millis();
    if(results.value != STAR.value && results.value != CONTINUOUS.value){followLine = false;}
    if(results.value == UP.value){old_value.value = results.value; dir = 1;}
    if(results.value == DOWN.value){old_value.value = results.value; dir = 0;}
    if(results.value == LEFT.value){old_value.value = results.value; dir = 3;}
    if(results.value == RIGHT.value){old_value.value = results.value; dir = 2;}
    if(results.value == ONE.value){if(!radar){radar = true;}else{radar = false; servopos = 90; radarServo.write(servopos); STOPFORTHELOVEOFGOD = false; delay(15);}}
    if(results.value == TWO.value){if(spinServo){spinServo = false; servopos = 90; radarServo.write(servopos);}else{spinServo = true;}}
    if(results.value == STAR.value){if(followLine == true){followLine = false;}else{followLine = true;}}
    
    if(results.value == CONTINUOUS.value)
    { 
      //Serial.println("CONTINUOUS");
      if(old_value.value == UP.value)
      {
        dir = 1;
        speed = 255;
      }
      else if(old_value.value == DOWN.value)
      {
        dir = 0;
        speed = 255;
      }
      else if(old_value.value == LEFT.value)
      {
        dir = 3;
        speed = 255;
      }  
      else if(old_value.value == RIGHT.value)
      {
        dir = 2;
        speed = 255;
      }      
    }
    irrecv.resume(); // Receive the next value
  }
  if(followLine){
   followLineLogic();//Käyttää viivanseuraamis logiikkaa ajamiseen 
  }
  if((millis() - lastPress) >= timeoutDelay && followLine == false)
  {
    Stop();
  }
  
  driveTo(); //Ajaa dir mukaiseen suuntaa
  if(STOPFORTHELOVEOFGOD){
   if(dir == 1){
    Stop(); 
   }
  }
  //analogWrite(speedPinA, speed);
  //analogWrite(speedPinB, speed);
  analogWrite(generalSpeedPin, speed);
}

void Stop()
{
  speed = 0;
  dir = 4;
}

void driveTo(){
if(dir == 0)
  {
  digitalWrite(dir1PinA, HIGH); // Kääntää taaksepäin
  digitalWrite(dir2PinA, LOW);
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);
  }
  else if(dir == 1)
  {
  digitalWrite(dir1PinA, LOW); //Kääntää eteenpäin
  digitalWrite(dir2PinA, HIGH); 
  digitalWrite(dir1PinB, HIGH);
  digitalWrite(dir2PinB, LOW);
  }
  else if(dir == 2)
  {
  digitalWrite(dir1PinA, HIGH); // Kääntää vasempaan
  digitalWrite(dir2PinA, LOW);
  digitalWrite(dir1PinB, HIGH);
  digitalWrite(dir2PinB, LOW);
  }
  else if(dir == 3)
  {
  digitalWrite(dir1PinA, LOW); // Kääntää oikeaan
  digitalWrite(dir2PinA, HIGH);
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);
  }
  else
  {
    Stop();
  }
}

void doRadarThings(){
    digitalWrite(trigPin, LOW);
    //delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    //delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH, 10000); //Tunnistaa 8cm asti
    timestamp = millis();
    distance = (duration * 0.5) * 0.03436426116;
    Serial.println(distance);
    if(distance < crashTreshold && distance != 0)
    {
      STOPFORTHELOVEOFGOD = true;
      /*dir = 4;
      speed = 0;*/
    }else{
     STOPFORTHELOVEOFGOD = false;
    }
    if(spinServo){
      if(servopos < 180 && servoDir == 0) //Jos alle 180 & liikkuu ylös, +1
      {
        radarServo.write(servopos);
        servopos += servoStep;
      }
      else if(servopos > 0 && servoDir == 1) // Jos suunta alas & value yli 0, -1
      {
        radarServo.write(servopos);
        servopos -= servoStep;
      }
      else if(servopos == 180 || servopos == 0) //Jos 180 tai 0, käännä suunta
      {
        radarServo.write(servopos);
        if(servoDir == 0)
        {
          servoDir = 1;
          servopos -= servoStep;
        }
        else if(servoDir == 1)
        {
           servoDir = 0;
           servopos += servoStep;
        }
      }
    }
    //Jos alle 180 & liikkuu ylös, +1
    //Jos 180, käännä suunta
    //Jos alle 180 & liikkuu alas, -1
    //Jos 0, käännä suunta
    delay(15);
}

void followLineLogic(){
  leftSensorValue = digitalRead(lineReaderLeftPin); // Arvo on 0 kun valkoista ja 1 jos mustaa
    middleSensorValue = digitalRead(lineReaderMiddlePin);
    rightSensorValue = digitalRead(lineReaderRightPin);
    
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
  }

