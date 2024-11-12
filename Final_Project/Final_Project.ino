#include "Servo.h"
#include "dht11.h"
#include "NewPing.h"
#include <LiquidCrystal_I2C.h>

// --------------- Pin Definitions -------------- //
// ---------------- Servo
#define SERVO_PIN 2

// ---------------- DHT
#define DHT_PIN   3

// ---------------- Ultrasonic 
#define TRIGGER_PIN 22
#define ECHO_PIN 23
#define MAX_DISTANCE 200

// --------------- Global Variables ------------- //
// ------------- Servo
Servo head_servo;

// ------------- DHT
dht11 DHT;
int readDHT, temp, humidity;

// ------------- Ultrasonic
NewPing eyes(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
float speedOfSound, distance, duration;

// ------------- LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
int countdown = 60;


// --------------- Functions -------------------- //

// ---------------------------------------------- Setup 
void setup() {
  Serial.begin(9600);
  head_servo.attach(SERVO_PIN);
  lcd.begin();
  lcd.backlight();

  
}

ISR(TIMER5_COMPB_vect){
  Serial.println("Hello From Interrupt");
}

// ---------------------------------------------- Head Turn
void Turn_Head(int angle){
  head_servo.write(angle);
}

// ---------------------------------------------- Detect Distance
int Detect_Distance(){

  readDHT = DHT.read(ECHO_PIN);
  temp = DHT.temperature;
  humidity = DHT.humidity;

  speedOfSound = 331.4 + (0.6 * temp) + (0.0124 * humidity);

  duration = eyes.ping_median(10); // 10 interations - returns duration in microseconds
  duration = duration/1000000; // Convert mircroseconds to seconds
  distance = (speedOfSound * duration)/2;
  distance = distance * 100; // meters to centimeters

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");

  return int(distance);
}

// ---------------------------------------------- Countdown Display
void firstLine(uint8_t offset){
  uint8_t colOffset = 0;
  uint8_t rowOffset = 0;
  lcd.setCursor(colOffset, rowOffset + offset);
}
void secondLine(uint8_t offset){
  uint8_t colOffset = 0;
  uint8_t rowOffset = 1;
  lcd.setCursor(colOffset, rowOffset + offset);
}

void Countdown_Display(){
  firstLine(0);
  lcd.print("Countdown: ");
  lcd.print(countdown);
}



void loop() {

  Turn_Head(0);

  Detect_Distance();

  Countdown_Display();

  delay(100);

}
