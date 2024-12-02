#include <Arduino.h>
#include "pinout.h"
#include "variables.h"
#include <ArduinoJson.h>
#include "SDcode.h"
#include <time.h>
#include <WiFi.h>

void NTPConnect()
{
  WiFi.begin(ssid, pw);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  configTime(gmtOffset_sec,daylightOffset_sec,ntpServer);
}

String getTimestamp()
{
  struct tm timeInfo;
  if (!getLocalTime(&timeInfo))
    Serial.println("Failed to obtain local time");
  char Timeformat[50];                   //char array/string for storing the formatted date
  strftime(Timeformat, 50, "%d-%m-%G %T", &timeInfo);  //strfttime formatting
  String timeStamp = Timeformat;
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
  File file = SD.open("/lightData.txt");                     //create a file on SD card and open the file
  if(!file)
  {
    writeFile(SD,"/lightData.txt","Timestamp            Light \r\n"); // \r\n means go to next line in the file
    Serial.println("Creating file...");
    file.close();                                      //close the file
  }
}

void setup() 
{
  Serial.begin(57600);
  //Serial.println("Begin.");
  delay(1000);
  SPI.begin(Sck,Miso,Mosi,csPin); //SCK,MISO,MOSI,cs pins
  while (!SD.begin(csPin))                                       
  {
    Serial.println("Error occurred at SD begin");
    delay(1000);
  }

  //SD
  newFileCreate();

  //NTP server
  NTPConnect();
  
  //sensor and timer
  pinMode(lightPin, INPUT);
  startTime = millis();
  //Serial.println("check 1 successful");

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
      //Serial.println(doc.memoryUsage());
      doc["Reading"]["Timestamp"] = getTimestamp();
      //testing
      Serial.println(getTimestamp()); 
      doc["Reading"]["light"] = lightValue();
      //testing
      Serial.println(light); 
      dataMessage = getTimestamp() + "  " + String(light) +"\r\n";
      appendFile(SD,"/lightData.txt",dataMessage.c_str());
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
      deleteFile(SD, "/lightData.txt");
      newFileCreate();
      startTime = currentTime; //start the new cycle
      
    }
    
  }
}
