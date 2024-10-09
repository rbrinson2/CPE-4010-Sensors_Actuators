
const int POT_MAX = 1023;
const int POT_MIN = 0;
const int FREQ_MAX = 10000;
const int FREQ_MIN = 60;

const float SLOPE = float(FREQ_MAX - FREQ_MIN) / float(POT_MAX - POT_MIN);


int buzzer=8;// select digital IO pin for the buzzer
int pot = 0;

void setup() { 
  Serial.begin(9600);
  pinMode(buzzer,OUTPUT); // set digital IO pin pattern, OUTPUT to be output 
} 



void loop() { 
  
  float val = 0;
  int freq = 0;
  while(1) 
  { 
    val = analogRead(pot);

    // Quick equation to find the frequency given the 
    // setting of the potentieometer
    freq = int(SLOPE * val + FREQ_MIN);

    // Print the resulting frequency
    Serial.print("Frequency = ");
    Serial.print(freq);
    Serial.println("Hz");

    // Function to output the desired frequency
    tone(buzzer, freq, 250);
  } 
} 