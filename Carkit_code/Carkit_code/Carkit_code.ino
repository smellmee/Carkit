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
int generalSpeedPin = 6;

//Nopeus & Suunta
int speed;
int speedBoost = 0;
int dir;
//Kaukosäädin
int RECV_PIN = 11;
long unsigned int IRcode, gap, lastPress, timeoutDelay;
IRrecv irrecv(RECV_PIN);
//harpake
Servo radarServo;
int servoPin = 10;
int servopos = 90;
int servoDir = 0;
int servoStep = 10;
int servolimit = 0;
boolean radar = false;
boolean STOPFORTHELOVEOFGOD = false;
boolean spinServo = false;

int rightDirectionDistance;
int leftDirectionDistance;

//Ultraäänisensori
int trigPin = 13;
int echoPin = 12;
unsigned long timestamp = 0;
long duration, distance;
int crashTreshold = 15;
decode_results results, OK, UP, DOWN, RIGHT, LEFT, ONE, TWO, THREE, STAR, CONTINUOUS, STOP, old_value, HASHTAG, ZERO, EIGHT;

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
  //Serial.print("AT+NAMESanik"); //Nimeää bluetooth laitteen
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
  HASHTAG.value = 16732845;
  ZERO.value = 16730805;
  EIGHT.value = 16726215;
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
 
  
  if(radar)
  {
    driveStraightWhileScanning(); 
    rightDirectionDistance = 0;
    leftDirectionDistance = 0;
  }
  
  if (irrecv.decode(&results)) 
  {
    
    //Serial.println(results.value);
    lastPress = millis();
    if(results.value != STAR.value && results.value != CONTINUOUS.value && results.value != HASHTAG.value && results.value != ZERO.value){followLine = false;}
    if(results.value == UP.value){old_value.value = results.value; dir = 1;}
    if(results.value == DOWN.value){old_value.value = results.value; dir = 0;}
    if(results.value == LEFT.value){old_value.value = results.value; dir = 3;}
    if(results.value == RIGHT.value){old_value.value = results.value; dir = 2;}
    if(results.value == ONE.value){if(!radar){radar = true;}else{radar = false; servopos = 90; radarServo.write(servopos); delay(15);}}
    if(results.value == TWO.value){if(spinServo){spinServo = false; servopos = 90; radarServo.write(servopos);}else{spinServo = true;}}
    if(results.value == STAR.value){if(followLine == true){followLine = false;}else{followLine = true;}}
    if(results.value == HASHTAG.value){speedBoost+=10;}
    if(results.value == ZERO.value){speedBoost-=10;}
    if(results.value == EIGHT.value){if(lineToFollow == 0){lineToFollow = 1;}else{lineToFollow = 0;}}
    Serial.println(lineToFollow);
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
   if(Serial.available())
  {//if there is data being recieved
    char btdir = Serial.read(); //read it
    dir = btdir - '0';
    speed = 255;
    driveTo(); //Ajaa dir mukaiseen suuntaa
    lastPress = millis();
  }
  driveTo(); //Ajaa dir mukaiseen suuntaa
 /* if(STOPFORTHELOVEOFGOD){
   if(dir == 1){
    Stop(); 
   }
  }*/
  //analogWrite(speedPinA, speed);
  //analogWrite(speedPinB, speed);
  int finalSpeed = speed + speedBoost; //Nopeuden säätöä varten tehty logiikka. Speedboost kertoo paljonko on manuaalisesti lisätty nopeutta
  if(finalSpeed > 255){
   finalSpeed = 255; 
  }else if(finalSpeed <0){
   finalSpeed = 0; 
  }
  analogWrite(generalSpeedPin, finalSpeed);
}

void Stop()
{
  speed = 0;
  speedBoost = 0;
  dir = 4;
}

void Stop2()
{
  speed = 0;
  speedBoost = 0;
  analogWrite(generalSpeedPin, speed);
  delay(500);
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
  digitalWrite(dir1PinA, HIGH); // Kääntää oikeaan
  digitalWrite(dir2PinA, LOW);
  digitalWrite(dir1PinB, HIGH);
  digitalWrite(dir2PinB, LOW);
  }
  else if(dir == 3)
  {
  digitalWrite(dir1PinA, LOW); // Kääntää vasempaan
  digitalWrite(dir2PinA, HIGH);
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);
  }
  else
  {
    Stop();
  }
}

void driveStraightWhileScanning()	{    //Ajaa suoraan scannaten

    Serial.println("driveStraightWhileScanning");
    speed = 120; 
    digitalWrite(dir1PinA, LOW); //Ajaa eteenpäin
    digitalWrite(dir2PinA, HIGH); 
    digitalWrite(dir1PinB, HIGH);
    digitalWrite(dir2PinB, LOW);
  
    analogWrite(generalSpeedPin, speed);
    
    doRadarThings();
}

void doRadarThings()	{
	  
    spinServo = false;
    digitalWrite(trigPin, LOW);
    //delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    //delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH, 10000); //Tunnistaa 8cm asti
    timestamp = millis();
    distance = (duration * 0.5) * 0.03436426116;
    //Serial.println(distance);
    if(distance < crashTreshold && distance != 0) //Seinä edessä
    {
	  Stop2();
      servopos = 90;
      radarServo.write(servopos);
      spinServo = true;
      }
      else{
     driveStraightWhileScanning();
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
          leftDirectionDistance = (duration * 0.5) * 0.03436426116;
          servoDir = 1;
          servopos -= servoStep;
          servolimit++;
          if(servolimit > 1){
          Serial.println(servolimit);
          radarServo.write(servopos);
          determineDirection();
        }
        }
        else if(servoDir == 1)
        {
          rightDirectionDistance = (duration * 0.5) * 0.03436426116;
          servoDir = 0;
          servopos += servoStep;
          servolimit++;
          if(servolimit > 1){
          Serial.println(servolimit);
          radarServo.write(servopos);
          determineDirection();}
        }
      }
    }
    delay(15);
}

void determineDirection(){
  servolimit = 0;
  spinServo = false;
  //Serial.println(rightDirectionDistance);
  //Serial.println(leftDirectionDistance);
  servopos = 90;
  radarServo.write(servopos);
  if(rightDirectionDistance > leftDirectionDistance){
    //Käänny oikealle
      speed = 255;
      analogWrite(generalSpeedPin, speed);
      digitalWrite(dir1PinA, LOW); // Kääntää oikeaan
      digitalWrite(dir2PinA, HIGH);
      digitalWrite(dir1PinB, LOW);
      digitalWrite(dir2PinB, HIGH);
    delay(285);
	driveStraightWhileScanning();    
  }
  else if(rightDirectionDistance < leftDirectionDistance){
    //Käänny vasemmalle
	  speed = 255;
	  dir = 1;
      analogWrite(generalSpeedPin, speed);
      digitalWrite(dir1PinA, HIGH); // Kääntää vasempaan
      digitalWrite(dir2PinA, LOW);
      digitalWrite(dir1PinB, HIGH);
      digitalWrite(dir2PinB, LOW);
    //palaa driveStraightWhileScanning tilaan
    delay(285);
    driveStraightWhileScanning();
  }
  else {
      //Käänny oikealle
      speed = 255;
      analogWrite(generalSpeedPin, speed);
      digitalWrite(dir1PinA, HIGH); // Kääntää oikeaan
      digitalWrite(dir2PinA, LOW);
      digitalWrite(dir1PinB, HIGH);
      digitalWrite(dir2PinB, LOW);
    delay(285);
	driveStraightWhileScanning();    
    //determineDirection();
    //driveStraightWhileScanning();
  } 
}

void followLineLogic(){
  leftSensorValue = digitalRead(lineReaderLeftPin); // Arvo on 0 kun valkoista ja 1 jos mustaa
    middleSensorValue = digitalRead(lineReaderMiddlePin);
    rightSensorValue = digitalRead(lineReaderRightPin);
    
    //Käännä vaan toista takapuolta
    if(rightSensorValue == lineToFollow && leftSensorValue != lineToFollow && middleSensorValue != lineToFollow)
    {
     //Kääntyy vasempaan 
     dir = 2;
     speed = 180;
    }else if(leftSensorValue == lineToFollow && rightSensorValue != lineToFollow && middleSensorValue != lineToFollow){
      //Kääntyy oikealle
      dir = 3;
      speed = 180;
    }else if(middleSensorValue != lineToFollow && rightSensorValue != lineToFollow && leftSensorValue != lineToFollow){
     //Sensori on kadottanut valkoisen viivan kokonaan. Ajaa taaksepäin? 

     dir = 0;
     speed = 115;
    }else{
    //Joko kaikki sensorit näkee viivan tai pelkästään keskimmäinen sensori näkee viivan
    //Auto ajaa siis eteenpäin 
    dir = 1;
    speed = 115;
    }
    //speed = 255;
    //analogWrite(generalSpeedPin, speed);
    //delay(50);
    //analogWrite(speedPinB, speed);
  }

