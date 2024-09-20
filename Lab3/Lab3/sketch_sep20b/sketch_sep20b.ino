
#define MAXPRESS 5
#define SECONDS 1000

int red = 11;// initialize pin 11
int green = 4;
int inpin = 7;// initialize pin 7
int val;// define val
bool pressFlag = false;
int pressCount = 1;


void setup()
{
  Serial.begin(9600);
  pinMode(red,OUTPUT);// set LED pin as “output”
  pinMode(green, OUTPUT); 
  pinMode(inpin,INPUT);// set button pin as “input”

}
void loop()
{
  val = digitalRead(inpin);// read the level value of pin 7 and assign if to val

  if (val == HIGH){
    pressFlag = true;
    digitalWrite(red, LOW);
    delay(pressCount * SECONDS);
    digitalWrite(red, HIGH);
  }
  else {
    digitalWrite(red, HIGH);
  }

  if ((val == LOW) && pressFlag){
    pressFlag = false;
    digitalWrite(green, LOW);
    delay(pressCount * SECONDS);
    digitalWrite(green, HIGH);

    Serial.print(pressCount);
    pressCount++;
    if (pressCount > 5) pressCount = 1;
  }
  else {
    digitalWrite(green, HIGH);
  }
}