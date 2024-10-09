
const int POT_MAX = 1023;
const int POT_MIN = 0;
const int FREQ_MAX = 10000;
const int FREQ_MIN = 60;
const int PERIOD_60HZ = 16667; // microseconds
const int PERIOD_10KHZ = 100; // microseconds

const float SLOPE = float(FREQ_MAX - FREQ_MIN) / float(POT_MAX);


int buzzer=8;// select digital IO pin for the buzzer
int pot = 0;

void setup() { 
  Serial.begin(9600);
  pinMode(buzzer,OUTPUT); // set digital IO pin pattern, OUTPUT to be output 
} 



void loop() { 
  unsigned char i,j;//define variable
  float val = 0;
  int freq = 0;
  while(1) 
  { 
    val = analogRead(pot);
    freq = int(SLOPE * val + FREQ_MIN);
    Serial.print("Frequency = ");
    Serial.print(freq);
    Serial.println("Hz");

    tone(buzzer, freq, 250);
  } 
} 