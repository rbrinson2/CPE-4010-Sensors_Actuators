
const int POT_MAX = 1023;
const int POT_MIN = 0;
const int FREQ_MAX = 10000;
const int FREQ_MIN = 60;
const int PERIOD_60HZ = 16667; // microseconds
const int PERIOD_10KHZ = 100; // microseconds

const float SLOPE = float(PERIOD_10KHZ - PERIOD_60HZ) / float(POT_MAX);


int buzzer=8;// select digital IO pin for the buzzer
int pot = 0;

void setup() { 
  Serial.begin(9600);
  pinMode(buzzer,OUTPUT); // set digital IO pin pattern, OUTPUT to be output 
} 



void loop() { 
  unsigned char i,j;//define variable
  float val = 0;
  int period = 0;
  while(1) 
  { 
    val = analogRead(pot);
    period = int(SLOPE * val + PERIOD_60HZ);

    Serial.print("Period: ");
    Serial.print(period);
    Serial.print("us\tAnalogue Value: ");
    Serial.println(val);

    for (int i = 0; i < period; i++) {
      digitalWrite(buzzer,HIGH);// sound
      delayMicroseconds(1);
      digitalWrite(buzzer,LOW);//not sound
      delayMicroseconds(1);
    }
    
    
  } 
} 