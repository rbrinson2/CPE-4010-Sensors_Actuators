#include "dht11.h"
#include "NewPing.h"
#include <LiquidCrystal_I2C.h>

// --------------- Pin Definitions -------------- //
// ---------------- Head Turn Timer


// ---------------- Servo
#define TURN_COMPLETE 30
#define TURN_DELAY 100

#define HEAD_PIN 2
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
#define RED_LED_PIN 9
#define GREEN_LED_PIN 8

// ---------------- Button
#define BUTTON_PIN 18

// ---------------- Photo Resistor
#define PHOTO_PIN 7
#define PHOTO_THRESH 750

// ---------------- First Look
#define FIRST true

// ---------------- Light Status
#define RED_LIGHT false
#define GREEN_LIGHT true

// --------------- Global Variables ------------- //
// ------------- Status 
enum GS {GAME_READY, GAME_ON, GAME_OVER};
volatile GS game_status = GAME_READY;

bool light_status;

enum WS {WIN, LOSE, PLAYING, STANDBY};
volatile WS win_status = STANDBY;

// ------------- Servo
int pulsewidth;
bool look_flag = FIRST;

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

// ---------------- Photo Resistor
int photo_res = 0;


// --------------- Functions -------------------- //
// ---------------------------------------------- Head Turn
void Head_Turn(int angle)// define a servo pulse function
{
  pulsewidth = (angle * 11) + 500;// convert angle to 500-2480 pulse width
  digitalWrite(HEAD_PIN, HIGH);  // set the level of servo pin as “high”
  delayMicroseconds(pulsewidth);  // delay microsecond of pulse width
  digitalWrite(HEAD_PIN, LOW);   // set the level of servo pin as “low”
  delay(20-pulsewidth/1000);
}

// ---------------------------------------------- Arm Turn
void Arm_Turn(int angle)// define a servo pulse function
{
  pulsewidth = (angle * 11) + 500;// convert angle to 500-2480 pulse width
  digitalWrite(ARM_PIN, HIGH);  // set the level of servo pin as “high”
  delayMicroseconds(pulsewidth);  // delay microsecond of pulse width
  digitalWrite(ARM_PIN, LOW);   // set the level of servo pin as “low”
  delay(20-pulsewidth/1000);
}

// ---------------------------------------------- Start Game
void Start_Game(){
  switch (game_status) {
    case GAME_READY: 
      game_status = GAME_ON;
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
      if (countdown > 0) {
        countdown--;
        if (countdown % 4 == 0) light_status = !light_status;
      }
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

  return distance;
}

// ---------------------------------------------- Photo Resistor
void Check_Photo(){
  if (photo_res > 750) {
    game_status = GAME_OVER;
    win_status = WIN;
  }
}

// ---------------------------------------------- Green Light Logic
void Green_Light_logic() {
  Check_Photo();

  for (int i = 0; i < TURN_COMPLETE; i++) Head_Turn(TURN_AWAY);
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);
  first_look = 0;
  look_flag = FIRST;
}

// ---------------------------------------------- Red Light Logic
void Red_Light_logic(){
  Check_Photo();

  if (look_flag == FIRST){
    for (int i = 0; i < TURN_COMPLETE; i++) Head_Turn(TURN_T0);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
    first_look = Detect_Distance();
    movement_grace = first_look * 0.3;
    look_flag = !FIRST;
  }
  else{
    Detect_Distance();
    if (distance >= (first_look - movement_grace) && distance <= (first_look + movement_grace));
    else {
      game_status = GAME_OVER;
      win_status = LOSE;
    }
  }
}
// ---------------------------------------------- Game Read Logic
void Game_Ready_Logic(){
  Head_Turn(TURN_AWAY);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  first_look = 0;
  look_flag = FIRST;
}

// ---------------------------------------------- Lose Logic
void Lose_Logic(){
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  Arm_Turn(ARM_STRIKE);
  secondLine(0);
  lcd.print("You Lose");
}

// ---------------------------------------------- Win Logic
void Win_Logic(){
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  secondLine(0);
  lcd.print("You Win!");
}

// ---------------------------------------------- Setup 
void setup() {
  // ----- Serial ----- //
  Serial.begin(115200);

  // ----- Timer ----- //
  Timer_Interrupt_Init();

  // ----- Button ----- //
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), Start_Game, RISING);

  // ----- Servo Initialize ----- //
  pinMode(HEAD_PIN, OUTPUT);

  // ----- LCD Intialize ----- //
  lcd.begin();
  lcd.backlight();

  // ----- LED Intialize ----- //
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  
}

// ---------------------------------------------- Loop
void loop(){ 
  photo_res = analogRead(PHOTO_PIN);
  Countdown_Display();

  switch (game_status) {
    case GAME_READY:
      Game_Ready_Logic();
    break;

    case GAME_ON:
      switch (light_status) {
        case GREEN_LIGHT:
          Green_Light_logic();
        break;

        case RED_LIGHT:
          Red_Light_logic();
        break;
      }
    break;

    case GAME_OVER:
      switch (win_status) {
        case WIN:
          Win_Logic();
        break;

        case LOSE:
          Lose_Logic();
        break;
      }
    break;   

  }

  delay(100);
  
}
