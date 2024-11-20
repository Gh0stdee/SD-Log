#include <FS.h>
#include <SD.h>
#include <SPI.h>


bool createDirectory(fs::FS &fs, const char* path)
{
    if(fs.mkdir(path))
    {
        Serial.println("created directory");
        return true;
    }
    else
    {
        Serial.println("Failed to make directory");
        delay(1000);
        return false;
    }
}

void deleteFile(fs::FS &fs, const char* path)
{
    if(fs.remove(path))
    {
        Serial.println("File deleted.");
    }
    else
    {
        Serial.println("Failed to delete file.");
    }
}

void writeFile(fs::FS &fs, const char* path, const char* message)
{
    Serial.println("Writing file.....");
    File data = fs.open(path,FILE_WRITE);
    if(data.print(message))
    {
        Serial.println("Data written.");
    }
    else
    {
        Serial.println("Writing failed.");
    }
    data.close();
}

void appendFile(fs::FS &fs, const char* path, const char* message)
{
    Serial.println("Appending file.....");
    File data = fs.open(path,FILE_APPEND);
    if(data.print(message))
    {
        Serial.println("Message appended.");
    }
    else
    {
        Serial.println("Message not appended.");
    }
    data.close();
}

