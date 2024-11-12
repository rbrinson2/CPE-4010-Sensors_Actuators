#include "Servo.h"

#define SERVO_PIN 2

Servo head_servo;

void setup() {
  Serial.begin(9600);
  head_servo.attach(SERVO_PIN);
}

// --------------- Functions -------------------- //

// --------------------------------- Head Turn -- //
void turn_head(int angle){
  head_servo.write(angle);
}



void loop() {


}
