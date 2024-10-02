#include <dht11.h>

dht11 DHT;
#define DHT11_PIN 4

float seconds = 0;
float deltaTemp = 0.0;

void setup(){
  Serial.begin(9600);
  Serial.println("\nDHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT11LIB_VERSION);
  Serial.print("DHT Check: ");
  int chk = DHT.read(DHT11_PIN);
  switch (chk){
    case DHTLIB_OK:  
                Serial.print("OK,\t"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.print("Checksum error,\t"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.print("Time out error,\t"); 
                break;
    default: 
                Serial.print("Unknown error,\t"); 
                break;
  }

  Serial.println();
  Serial.println("\n---- Temperature ----");
}

void tempPrint(int temp){

  Serial.print(", C: ");
  Serial.print(temp,1);
  Serial.print(", F: ");
  Serial.print(fahrenheit(temp), 1);
  Serial.print(", K: ");
  Serial.println(kelvin(temp), 1);
}

float fahrenheit(float cel){
  return cel * (9/5)  + 32;
}

float kelvin(float cel){
  return cel + 273.15;
}

void loop(){
  DHT.read(DHT11_PIN);
  float temp = DHT.temperature;

  if (temp > deltaTemp){
    deltaTemp = temp;
    Serial.print("Time: ");
    Serial.print(seconds, 1);
    tempPrint(deltaTemp);
  }
  
  delay(100);
  seconds += 0.1;
}