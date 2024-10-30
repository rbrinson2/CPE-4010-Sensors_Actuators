#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <stdint-gcc.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

int waterSensorPin = 7;

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


void setup()
{
  Serial.begin(9600);
	// initialize the LCD
	lcd.begin();

	// Turn on the blacklight and print a message.
	lcd.backlight();

  lcd.print("Ryan");
}

void loop()
{ 
  int waterLevel = analogRead(waterSensorPin);
  Serial.println(waterLevel);

  
  if (waterLevel <= 1){
    firstLine(0);
    lcd.clear();
    lcd.print("Dry");
  }
  else if (waterLevel > 0 && waterLevel < 600) {
    firstLine(0);
    lcd.clear();
    lcd.print("Low");
  }
  else if (waterLevel >= 600 && waterLevel < 700) {
    firstLine(0);
    lcd.clear();
    lcd.print("Medium");
  }
  else if (waterLevel >= 700) {
    firstLine(0);
    lcd.clear();
    lcd.print("Full");
  }
  else {
    firstLine(0);
    lcd.clear();
    lcd.print("Error");
  }

  secondLine(0);
  lcd.print(waterLevel);

  delay(1000);
}
