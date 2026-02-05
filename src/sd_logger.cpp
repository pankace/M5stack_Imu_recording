/**
 * @file sd_logger.cpp
 * @brief SD card logging implementation
 * 
 * @author pankace
 * @date 2026-02-05
 * @version 1.0
 */

#include "sd_logger.h"
#include "config.h"

void appendFile(fs::FS& fs, const char* path, const char* message)
{
    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("ERROR: Failed to open file for appending");
        M5.Lcd.fillScreen(RED);
        return;
    }
    
    if (!file.print(message))
    {
        Serial.println("ERROR: Append operation failed");
    }
    
    file.close();
}

void writeFile(fs::FS& fs, const char* path, const char* message)
{
    Serial.printf("Writing file: %s\n", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("ERROR: Failed to open file for writing");
        M5.Lcd.fillScreen(RED);
        return;
    }
    
    if (file.print(message))
    {
        Serial.println("INFO: File written successfully");
    }
    else
    {
        Serial.println("ERROR: Write operation failed");
        M5.Lcd.fillScreen(RED);
    }
    
    file.close();
}

void initializeDataFiles(void)
{
    writeFile(SD, FILE_ACCELERATION, "millis,aX,aY,aZ");
    writeFile(SD, FILE_GYROSCOPE, "millis,gX,gY,gZ");
    writeFile(SD, FILE_MAGNETOMETER, "millis,mX,mY,mZ");
    writeFile(SD, FILE_QUATERNION, "millis,q0,qX,qY,qZ");
    writeFile(SD, FILE_YPR, "millis,rate_hz,Yaw,Pitch,Roll");
    writeFile(SD, FILE_DIAGNOSTICS, "SelfTest,GyroBias,AccelBias,MagCalibration");
}
