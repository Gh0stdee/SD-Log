#include <Arduino.h>
#include <ArduinoJson.h>
#include "SDcode.h"
#include "pinout.h"
#include "variables.h"

DynamicJsonDocument doc (2*size_1mb) ; //allocate heap memory(value) to create Json document
String dataMessage;

//get light value
int lightValue()
{
  light = analogRead(lightPin);
  return light;
}  

//create file if file doesn't exist
void newFileCreate()
{
  File file = SD.open("/data.txt");                     //create a file on SD card and open the file
  if(!file)
  {
    writeFile(SD,"/data.txt","Timestamp, Light \r\n"); // \r\n means go to next line in the file
    file.close();                                      //close the file
  }
}

void setup() 
{
  Serial.begin(9600); 

  //SD
  SD.begin(csPin);                                       //initiate spi communication with chip select pin
  SPI.begin(sck,MISO,MOSI,csPin);
  createDirectory(SD,"newdir");
  newFileCreate();
  
  //sensor and timer
  pinMode(lightPin, INPUT);
  startTime = millis();
}

/*
operator
doc[key] = value -> "key" : value
*/
void loop() {
  currentTime = millis();
  if ((currentTime-startTime) >= 1000)
  {
    if (doc.memoryUsage() <= size_1mb)    
    {
      doc["Reading"]["Timestamp"] = currentTime;
      //testing
      Serial.println(currentTime); 
      doc["Reading"]["light"] = lightValue();
      //testing
      Serial.println(light); 
      dataMessage = String(currentTime) + "," + String(light) +"\r\n";
      appendFile(SD,"/data.txt",dataMessage.c_str());
      //testing
      Serial.print("Data saved(TimeStamp, Light): ");
      Serial.println(currentTime, light);
      startTime = currentTime; //start the new cycle
      
    }
    else //exceeded 1mb size
    {
      //testing
      Serial.println("Exceeded predetermined size"); 
      doc.clear();
      deleteFile(SD, "/data.txt");
      newFileCreate();
    }
    
  }
}

