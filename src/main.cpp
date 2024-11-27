#include <Arduino.h>
#include "pinout.h"
#include "variables.h"
#include <ArduinoJson.h>
#include "SDcode.h"
#include <time.h>

//TimeStamp Fomat Min:Sec
String getTimestamp()
{
  time_t timeInfo = time(NULL);          //time_t for timestamps
  struct tm date =*localtime(&timeInfo); //struct tm for datatime struct and access to members
  char Timeformat[50];                   //char array/string for storing the formatted date
  strftime(Timeformat, 50, "%H:%M:%S", &date);  //strfttime formatting
  String timeStamp = Timeformat;
  return timeStamp;
}

//get light value
int lightValue()
{
  light = analogRead(lightPin);
  return light;
}  
DynamicJsonDocument doc (2*1024) ; //size_1mb
String dataMessage;

//create file if file doesn't exist
void newFileCreate()
{
  File file = SD.open("/data.txt");                     //create a file on SD card and open the file
  if(!file)
  {
    writeFile(SD,"/lightData.txt","Timestamp Light \r\n"); // \r\n means go to next line in the file
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
    if (doc.memoryUsage() <= 2048)    //size_1mb
    {
      Serial.print(doc.memoryUsage());
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
