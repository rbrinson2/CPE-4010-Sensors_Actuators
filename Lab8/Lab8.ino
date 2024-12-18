/*
    MPU6050 Triple Axis Gyroscope & Accelerometer. Simple Accelerometer Example.
    Read more: http://www.jarzebski.pl/arduino/czujniki-i-sensory/3-osiowy-zyroskop-i-akcelerometr-mpu6050.html
    GIT: https://github.com/jarzebski/Arduino-MPU6050
    Web: http://www.jarzebski.pl
    (c) 2014 by Korneliusz Jarzebski
*/

#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

#define Max_Left      47
#define Left          48
#define Center_Left   49
#define Center        50
#define Center_Right  51
#define Right         52
#define Max_Right     53

void setup() 
{
  Serial.begin(115200);

  Serial.println("Initialize MPU6050");

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }

  // If you want, you can set accelerometer offsets
  // mpu.setAccelOffsetX();
  // mpu.setAccelOffsetY();
  // mpu.setAccelOffsetZ();
  
  checkSettings();

  pinMode(Max_Left, OUTPUT);
  pinMode(Left, OUTPUT);
  pinMode(Center_Left, OUTPUT);
  pinMode(Center, OUTPUT);
  pinMode(Center_Right, OUTPUT);
  pinMode(Right, OUTPUT);
  pinMode(Max_Right, OUTPUT);
  
}

void checkSettings()
{
  Serial.println();
  
  Serial.print(" * Sleep Mode:            ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");
  
  Serial.print(" * Clock Source:          ");
  switch(mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  
  Serial.print(" * Accelerometer:         ");
  switch(mpu.getRange())
  {
    case MPU6050_RANGE_16G:            Serial.println("+/- 16 g"); break;
    case MPU6050_RANGE_8G:             Serial.println("+/- 8 g"); break;
    case MPU6050_RANGE_4G:             Serial.println("+/- 4 g"); break;
    case MPU6050_RANGE_2G:             Serial.println("+/- 2 g"); break;
  }  

  Serial.print(" * Accelerometer offsets: ");
  Serial.print(mpu.getAccelOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getAccelOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getAccelOffsetZ());
  
  Serial.println();
}

void loop()
{
  Vector rawAccel = mpu.readRawAccel();
  Vector normAccel = mpu.readNormalizeAccel();

  // Serial.print(" Xraw = ");
  // Serial.print(rawAccel.XAxis);
  // Serial.print(" Yraw = ");
  // Serial.print(rawAccel.YAxis);
  // Serial.print(" Zraw = ");
  // Serial.println(rawAccel.ZAxis);

  float xraw = rawAccel.XAxis;
  float yraw = rawAccel.YAxis;
  float zraw = rawAccel.ZAxis;

  float xnorm = normAccel.XAxis;
  float ynorm = normAccel.YAxis;
  float znorm = normAccel.ZAxis;

  float roll = tan(xnorm / znorm);
  float rollDeg = (roll / (2 * PI)) * 360;

  
  Serial.print(" Xnorm = ");
  Serial.print(xnorm);
  Serial.print(" Ynorm = ");
  Serial.print(ynorm);
  Serial.print(" Znorm = ");
  Serial.println(znorm);

  Serial.print("X Roll = ");
  Serial.println(rollDeg);

  if (xnorm <= -19.5) digitalWrite(Max_Left, HIGH);
  else digitalWrite(Max_Left, LOW);
    
  if (xnorm > -19.5 && xnorm <= -13) digitalWrite(Left, HIGH); 
  else digitalWrite(Left, LOW);
  
  if (xnorm > -13 && xnorm <= -6.5) digitalWrite(Center_Left, HIGH);
  else digitalWrite(Center_Left, LOW);

  if (xnorm > -6.5 && xnorm < 6.5) digitalWrite(Center, HIGH);
  else digitalWrite(Center, LOW);
  
  if (xnorm >= 6.5 && xnorm < 13) digitalWrite(Center_Right, HIGH);
  else digitalWrite(Center_Right, LOW);

  if (xnorm >= 13 && xnorm < 19.5) digitalWrite(Right, HIGH);
  else digitalWrite(Right, LOW);

  if (xnorm >= 19.5) digitalWrite(Max_Right, HIGH);
  else digitalWrite(Max_Right, LOW);
  
  delay(1000);
}