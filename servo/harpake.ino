
int PWMPin = 3;
int powerPin = 5;
int kaannos = 2;

void setup()
{
pinMode(PWMPin, OUTPUT);
}

void loop()
{
  analogWrite(PWMPin,kaannos);
  if(kaannos == 2){
   kaannos = -5; 
  }else{
   kaannos = 2; 
  }
  
}
