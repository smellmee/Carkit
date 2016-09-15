
int PWMPin = 6;
int powerPin = 5;
int kaannos = 2;

void setup()
{
pinMode(PWMPin, OUTPUT);
Serial.begin(9600);
}

void loop()
{
  analogWrite(PWMPin,kaannos);
  if(kaannos == 2){
    Serial.println("LOL OK");
   kaannos = -2; 
  }else{
    Serial.println("opasfopafpsf");
   kaannos = 2; 
  }
  delay(600);
}
