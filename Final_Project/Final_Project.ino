#include "Servo.h"
#include "dht11.h"
#include "NewPing.h"
#include <LiquidCrystal_I2C.h>

// --------------- Pin Definitions -------------- //
// ---------------- Head Turn Timer
#define MAX

// ---------------- Servo
#define SERVO_PIN 2

// ---------------- DHT
#define DHT_PIN   3

// ---------------- Ultrasonic 
#define TRIGGER_PIN 22
#define ECHO_PIN 23
#define MAX_DISTANCE 200

// ---------------- Red/Green LEDs
#define RED_LIGHT 10
#define GREEN_LIGHT 11

// ---------------- Button
#define BUTTON_PIN 18

// --------------- Global Variables ------------- //
// ------------- Game Over
enum GS {GAME_READY, GAME_ON, GAME_OVER};
volatile GS game_status = GAME_READY;

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
// ---------------------------------------------- Countdown Interrupt
void Interrupt_On(){
  TCCR5A = 0;
  TCCR5B = 0;
  TCCR5B |= B00000100;
  TCNT5 = 3036;
  TIMSK5 |= B00000001;
}
void Interrupt_Off(){
  TIMSK5 &= B11111110;
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
  if (countdown == 9) lcd.clear();
  lcd.print("Countdown: ");
  lcd.print(countdown);
}



// ---------------------------------------------- Setup 
void setup() {
  // ----- Serial ----- //
  Serial.begin(9600);

  // ----- Button ----- //
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), Start_Game, LOW);

  // ----- Servo Initialize ----- //
  head_servo.attach(SERVO_PIN);

  // ----- LCD Intialize ----- //
  lcd.begin();
  lcd.backlight();

  // ----- Timer ----- //
  Interrupt_On();
}

void Start_Game(){
  game_status = GAME_ON;
}

ISR(TIMER5_OVF_vect){
  switch (game_status) {
    case GAME_READY:
      countdown = 60;
    break;
    case GAME_ON:
      if (countdown > 0) countdown--;
      else game_status = GAME_OVER;
    break;
    GAME_OVER:
      countdown = 0;
    break;
  }
  TCNT5 = 3036;
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



void loop(){ 
  Countdown_Display();
  
  delay(100);
}
