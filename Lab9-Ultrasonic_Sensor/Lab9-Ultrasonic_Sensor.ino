#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin
#define LEDPin 13 // Onboard LED

#define RED 50
#define YELLOW 51
#define GREEN 52

#define BUZZER 53

int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance

int distance_arr[20];
int arr_length = sizeof(distance_arr) / sizeof(int);
int average = 0;

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
  for (int led = RED; led <= GREEN; led++) pinMode(led, OUTPUT);
  pinMode(BUZZER, OUTPUT);
}

void freq(int d){
  int f = 0;
  if (d > 12) f = 0;
  else f = 200 * d + 500;
  // Serial.print("Frequency: ");
  // Serial.println(f);
  tone(BUZZER, f);
}

int avg_distance(int d){
  int sum = 0;
  int avg = 0;
  for (int i = arr_length; i > 0; i--){
    distance_arr[i] = distance_arr[i - 1];
  }
  distance_arr[0] = d;
  
  for (int j = 0; j < arr_length; j++){
    sum += distance_arr[j];
  }

  avg = sum / arr_length;

  return avg;
}

void loop() {
  /* The following trigPin/echoPin cycle is used to determine the
  distance of the nearest object by bouncing soundwaves off of it. */ 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;

  average = avg_distance(distance);

  freq(average);

  Serial.print("Distance: ");
  if (average > maximumRange) Serial.println("-1");
  else Serial.println(average);
 
  if (average >= 12) {
    digitalWrite(GREEN, HIGH);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, LOW);
  }
  else if (average >= 5 && average < 12) { 
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(RED, LOW);
  }
  else if (average < 5) {
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(RED, HIGH);
  }


  //Delay 50ms before next reading.
  delay(50);
}