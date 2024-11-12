#include "Servo.h"
#include "dht11.h"

#define SERVO_PIN 2


// --------------- Global Variables ------------- //
Servo head_servo;





// --------------- Functions -------------------- //

// ---------------------------------------------- Setup 
void setup() {
  Serial.begin(9600);
  head_servo.attach(SERVO_PIN);
}

// ---------------------------------------------- Head Turn
void Turn_Head(int angle){
  head_servo.write(angle);
}

// ---------------------------------------------- Detect Distance
int Detect_Distance(){
  int d = 0;
  return d;
}



void loop() {

  

  delay(50);

}
