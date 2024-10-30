#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <stdint-gcc.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

uint8_t count = 1;

void secondLine(uint8_t offset){
  uint8_t colOffset = 0;
  uint8_t rowOffset = 1;
  lcd.setCursor(colOffset, rowOffset + offset);
}


void setup()
{
	// initialize the LCD
	lcd.begin();

	// Turn on the blacklight and print a message.
	lcd.backlight();

  lcd.print("Ryan");
}

void loop()
{
  secondLine(0);
  lcd.print("%c", count);
}
