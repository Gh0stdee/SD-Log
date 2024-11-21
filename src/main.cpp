#include <Arduino.h>
#include "pinout.h"
#include "variables.h"
#include <ArduinoJson.h>
#include "SDcode.h"
#include <time.h>

//TimeStamp Fomat Min:Sec
String getTimestamp()
{
  time_t timeInfo = time(NULL);
  struct tm date =*localtime(&timeInfo);
  String minute = (String)date.tm_min;
  String second = (String)date.tm_sec;
  String timeStamp = (minute +":"+ second);
  return timeStamp;
}

//get light value
int lightValue()
{
  light = analogRead(lightPin);
  return light;
}  
DynamicJsonDocument doc (2*size_1mb) ;
String dataMessage;

//create file if file doesn't exist
void newFileCreate()
{
  File file = SD.open("/data.txt");                     //create a file on SD card and open the file
  if(!file)
  {
    writeFile(SD,"/data.txt","Timestamp, Light \r\n"); // \r\n means go to next line in the file
    Serial.println("Creating file...");
    file.close();                                      //close the file
  }
}

void setup() 
{
  Serial.begin(57600);
  Serial.println("Begin.");
  delay(1000);
  SPI.begin(16,5,17,4); //SCK,MISO,MOSI,csPin
  while (!SD.begin(4))                                       
  {
    Serial.println("Error occurred at SD begin");
    delay(1000);
  }
  //SD
  
  
  newFileCreate();
  
  //sensor and timer
  pinMode(lightPin, INPUT);
  startTime = millis();
  Serial.println("check 1 successful");

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
      Serial.println("check 2 successful");
      doc["Reading"]["Timestamp"] = getTimestamp();
      //testing
      Serial.println(currentTime); 
      doc["Reading"]["light"] = lightValue();
      //testing
      Serial.println(light); 
      dataMessage = String(currentTime) + "," + String(light) +"\r\n";
      appendFile(SD,"/data.txt",dataMessage.c_str());
      //testing
      //Serial.print("Data saved(TimeStamp, Light): ");
      //Serial.println(currentTime, light);
      
      startTime = currentTime; //start the new cycle
      
    }
    else //exceeded 1mb size
    {
      //testing
      Serial.println("Exceeded predetermined size"); 
      
      
      doc.clear();
      deleteFile(SD, "/data.txt");
      newFileCreate();
      startTime = currentTime; //start the new cycle
      
    }
    
  }
}
