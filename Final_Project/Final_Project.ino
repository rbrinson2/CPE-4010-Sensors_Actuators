#include "Servo.h"
#include "dht11.h"
#include "NewPing.h"
#include <LiquidCrystal_I2C.h>

// --------------- Pin Definitions -------------- //
// ---------------- Head Turn Timer


// ---------------- Servo
#define SERVO_PIN 2
#define TURN_AWAY 180
#define TURN_T0 0

#define ARM_PIN 10
#define ARM_WAIT 0
#define ARM_STRIKE 90

// ---------------- DHT
#define DHT_PIN   3

// ---------------- Ultrasonic 
#define TRIGGER_PIN 22
#define ECHO_PIN 23
#define MAX_DISTANCE 200

// ---------------- Red/Green LEDs
#define RED_LIGHT_PIN 10
#define GREEN_LIGHT_PIN 11

// ---------------- Button
#define BUTTON_PIN 18

// --------------- Global Variables ------------- //
// ------------- Status Enums
enum GS {GAME_READY, GAME_ON, GAME_OVER};
volatile GS game_status = GAME_READY;

enum LS {RED_LIGHT, GREEN_LIGHT, NO_LIGHT};
volatile LS light_status = NO_LIGHT;

enum WS {WIN, LOSE, PLAYING, STANDBY};
volatile WS win_status = STANDBY;

// ------------- Servo
Servo head_servo;
Servo arm_servo;
enum LF {FIRST, NOT_FIRST};
LF look_flag = NOT_FIRST;

// ------------- DHT
dht11 DHT;
int readDHT, temp, humidity;

// ------------- Ultrasonic
NewPing eyes(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
float speedOfSound, distance, duration;
float first_look, movement_grace;

// ------------- LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
int countdown = 60;


// --------------- Functions -------------------- //

// ---------------------------------------------- Start Game
void Start_Game(){
  switch (game_status) {
    case GAME_READY: 
      game_status = GAME_ON;
      Timer_Interrupt_Init();
    break;

    case GAME_ON: 
    break;

    case GAME_OVER:
      game_status = GAME_READY;
    break;
  }
}

// ---------------------------------------------- Timer ISR
ISR(TIMER5_OVF_vect){
  switch (game_status) {
    case GAME_READY:
      countdown = 60;
    break;
    case GAME_ON:
      if (countdown > 0) countdown--;
      else {
        countdown = 0;
        game_status = GAME_OVER;
        win_status = LOSE;
      }
    break;
    GAME_OVER:
      countdown = countdown;
    break;
  }
  TCNT5 = 3036;
}

// ---------------------------------------------- Countdown Interrupt
void Timer_Interrupt_Init(){
  TCCR5A = 0;
  TCCR5B = 0;
  TCCR5B |= B00000100;
  TCNT5 = 3036;
  TIMSK5 |= B00000001;
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

  return distance;
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
  arm_servo.attach(ARM_PIN);

  // ----- LCD Intialize ----- //
  lcd.begin();
  lcd.backlight();

  // ----- Timer ----- //
  //Timer_Interrupt_Init();
}

// ---------------------------------------------- Loop
void loop(){ 
  Countdown_Display();

  switch (game_status) {
    case GAME_READY:
      head_servo.write(TURN_T0);
    break;

    case GAME_ON:
      switch (light_status) {
        case GREEN_LIGHT:
          head_servo.write(TURN_AWAY);
        break;

        case RED_LIGHT:
          head_servo.write(TURN_T0);
          if (head_servo.read() == TURN_T0 && look_flag == FIRST){
            first_look = Detect_Distance();
            movement_grace = first_look * 0.1;
          }
          else{
            if (distance >= (first_look - movement_grace) && distance <= (first_look + movement_grace));
            else {
              game_status = GAME_OVER;
              win_status = LOSE;
            }
          }
        break;
      }
    break;

    case GAME_OVER:
      switch (win_status) {
        case WIN:
        break;

        case LOSE:
          arm_servo.write(ARM_STRIKE);
        break;
      }
    break;   

  }
  
  delay(100);
}
