#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

//sensor
#define lightPin 4

int light;
const int size_1mb = 1048576;     //1 mb size 
unsigned long startTime;
unsigned long currentTime;
DynamicJsonDocument doc (2*size_1mb) ; //allocate heap memory(value) to create Json document

/*
operator
doc[key] = value -> "key" : value
*/
int lightValue()
{
  light = analogRead(lightPin);
  return light;
}  

void setup() 
{
  startTime = millis();
  pinMode(lightPin, INPUT);
  Serial.begin(9600); //testing
}

void loop() {
  currentTime = millis();
  if ((currentTime-startTime) >= 1000)
  {
    if (doc.memoryUsage() <= size_1mb)    
    {
      doc["Reading"]["Timestamp"] = currentTime;
      Serial.println(currentTime); //testing
      doc["Reading"]["light"] = lightValue();
      Serial.println(light); //testing
      startTime = currentTime; //start the new cycle
    }
    else //exceeded 1mb size
    {
      doc.clear();
      Serial.println("Exceeded size"); //testing
    }
    
  }
}

