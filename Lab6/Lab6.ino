int servopin=9;// select digital pin 9 for servomotor signal line
int myangle;// initialize angle variable
int pulsewidth;// initialize width variable
int val;
int potPin = 0;

void servopulse(int servopin,int myangle)// define a servo pulse function
{
  pulsewidth=(myangle*11)+500;// convert angle to 500-2480 pulse width
  digitalWrite(servopin,HIGH);// set the level of servo pin as “high”
  delayMicroseconds(pulsewidth);// delay microsecond of pulse width
  digitalWrite(servopin,LOW);// set the level of servo pin as “low”
  delay(20-pulsewidth/1000);
}

void setup()
{
  pinMode(servopin,OUTPUT);// set servo pin as “output”
  Serial.begin(9600);// connect to serial port, set baud rate at “9600”
  Serial.println("servo=o_seral_simple ready" ) ;
}
void loop()// convert number 0 to 9 to corresponding 0-180 degree angle, LED blinks corresponding number of time
{
  int potInput = analogRead(potPin);

  potInput *= int(180/potInput);// convert number to angle
  Serial.print("moving servo to ");
  Serial.print(potInput,DEC);
  Serial.println();

  servopulse(servopin,potInput);// use the pulse function
  delay(1000);
}