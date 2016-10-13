#include <Servo.h>

/*T.V. Tatu Piippo 9v*/
#include <IRremote.h>
//motor A
int dir1PinA = 2;
int dir2PinA = 3;
//int speedPinA = 10;
//motor B
int dir1PinB = 4;
int dir2PinB = 5;
//int speedPinB = 10;
int generalSpeedPin = 6;
//Nopeus & Suunta
int speed;
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
int rightDirectionDistance;
int leftDirectionDistance;
boolean radar = false;
boolean STOPFORTHELOVEOFGOD = false;
boolean spinServo = false;
boolean autoDrive = true;
//Ultraäänisensori
int trigPin = 13;
int echoPin = 12;
unsigned long timestamp = 0;
long duration, distance;
int crashTreshold = 15;
decode_results results, OK, UP, DOWN, RIGHT, LEFT, ONE, TWO, THREE, STAR, CONTINUOUS, STOP, old_value;

void setup()	{
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
  STOP.value = 0;
  pinMode(dir1PinA, OUTPUT);
  pinMode(dir2PinA, OUTPUT);
  //pinMode(speedPinA, OUTPUT);
  pinMode (generalSpeedPin, OUTPUT);
  pinMode(dir1PinB, OUTPUT);
  pinMode(dir2PinB, OUTPUT);
  //pinMode(speedPinB, OUTPUT);
  pinMode(servoPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  speed = 200;
  dir = 4;
  irrecv.enableIRIn(); // Start the receiver
  timeoutDelay = 100;
  radarServo.attach(servoPin);
}

void loop() {
  	                                 	
	//driveStraightWhileScanning(); 
        rightDirectionDistance = 0;
        leftDirectionDistance = 0;	              
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
	  Stop();
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
  
void Stop()	{
  speed = 0;
  analogWrite(generalSpeedPin, speed);
  delay(500);
}

void determineDirection(){
  Serial.println(rightDirectionDistance);
  Serial.println(leftDirectionDistance);
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
      Serial.println("oikeaan");
    delay(270);
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
      Serial.println("vasempaan");
    //palaa driveStraightWhileScanning tilaan
    delay(270);
    driveStraightWhileScanning();
  }
  else {
    Serial.println("Yhtä suuret");
      //Käänny oikealle
      speed = 255;
      analogWrite(generalSpeedPin, speed);
      digitalWrite(dir1PinA, LOW); // Kääntää oikeaan
      digitalWrite(dir2PinA, HIGH);
      digitalWrite(dir1PinB, LOW);
      digitalWrite(dir2PinB, HIGH);
      Serial.println("oikeaan");
    delay(270);
	driveStraightWhileScanning();    
    //determineDirection();
    //driveStraightWhileScanning();
  } 
}
