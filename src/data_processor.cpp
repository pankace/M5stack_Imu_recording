/**
 * @file data_processor.cpp
 * @brief Data processing and AHRS implementation
 * 
 * @author pankace
 * @date 2026-02-05
 * @version 1.0
 */

#include "data_processor.h"
#include "imu_sensor.h"
#include "sd_logger.h"
#include "config.h"
#include "utility/quaternionFilters.h"
#include <M5Stack.h>

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

extern char msg[MSG_BUFFER_SIZE];

// ============================================================================
// DATA PROCESSING FUNCTIONS
// ============================================================================

void processBasicMode(void)
{
    imuSensor.delt_t = millis() - imuSensor.count;
    if (imuSensor.delt_t > BASIC_UPDATE_INTERVAL_MS)
    {
        if (SERIAL_DEBUG_ENABLED)
        {
            // Print acceleration values
            Serial.print("X-acceleration: ");
            Serial.print(1000 * imuSensor.ax);
            Serial.print(" mg  Y-acceleration: ");
            Serial.print(1000 * imuSensor.ay);
            Serial.print(" mg  Z-acceleration: ");
            Serial.print(1000 * imuSensor.az);
            Serial.println(" mg");

            // Print gyroscope values
            Serial.print("X-gyro rate: ");
            Serial.print(imuSensor.gx, 3);
            Serial.print(" deg/s  Y-gyro rate: ");
            Serial.print(imuSensor.gy, 3);
            Serial.print(" deg/s  Z-gyro rate: ");
            Serial.print(imuSensor.gz, 3);
            Serial.println(" deg/s");

            // Print magnetometer values
            Serial.print("X-mag field: ");
            Serial.print(imuSensor.mx);
            Serial.print(" mG  Y-mag field: ");
            Serial.print(imuSensor.my);
            Serial.print(" mG  Z-mag field: ");
            Serial.print(imuSensor.mz);
            Serial.println(" mG");

            // Print temperature
            imuSensor.tempCount = imuSensor.readTempData();
            imuSensor.temperature = ((float)imuSensor.tempCount) / TEMP_CONVERSION_FACTOR + TEMP_OFFSET;
            Serial.print("Temperature: ");
            Serial.print(imuSensor.temperature, 1);
            Serial.println(" °C");
            Serial.println();
        }
        
        imuSensor.count = millis();
    }
}

void calculateOrientation(void)
{
    imuSensor.yaw = atan2(2.0f * (*(getQ() + 1) * *(getQ() + 2) + *getQ() * *(getQ() + 3)),
                    *getQ() * *getQ() + *(getQ() + 1) * *(getQ() + 1) - 
                    *(getQ() + 2) * *(getQ() + 2) - *(getQ() + 3) * *(getQ() + 3));
    
    imuSensor.pitch = -asin(2.0f * (*(getQ() + 1) * *(getQ() + 3) - *getQ() * *(getQ() + 2)));
    
    imuSensor.roll = atan2(2.0f * (*getQ() * *(getQ() + 1) + *(getQ() + 2) * *(getQ() + 3)),
                     *getQ() * *getQ() - *(getQ() + 1) * *(getQ() + 1) - 
                     *(getQ() + 2) * *(getQ() + 2) + *(getQ() + 3) * *(getQ() + 3));
    
    // Convert to degrees
    imuSensor.pitch *= RAD_TO_DEG;
    imuSensor.yaw *= RAD_TO_DEG;
    imuSensor.yaw -= MAGNETIC_DECLINATION_DEG;
    imuSensor.roll *= RAD_TO_DEG;
}

void processAHRSMode(void)
{
    imuSensor.delt_t = millis() - imuSensor.count;
    
    if (imuSensor.delt_t > AHRS_UPDATE_INTERVAL_MS)
    {
        if (SERIAL_DEBUG_ENABLED)
        {
            // Log acceleration data
            Serial.print("ax = ");
            Serial.print((int)(1000 * imuSensor.ax));
            Serial.print(" ay = ");
            Serial.print((int)(1000 * imuSensor.ay));
            Serial.print(" az = ");
            Serial.print((int)(1000 * imuSensor.az));
            Serial.println(" mg");
            
            snprintf(msg, MSG_BUFFER_SIZE, "\r\n%lu,%lf,%lf,%lf", 
                    millis(), 1000 * imuSensor.ax, 1000 * imuSensor.ay, 1000 * imuSensor.az);
            appendFile(SD, FILE_ACCELERATION, msg);

            // Log gyroscope data
            Serial.print("gx = ");
            Serial.print(imuSensor.gx, 2);
            Serial.print(" gy = ");
            Serial.print(imuSensor.gy, 2);
            Serial.print(" gz = ");
            Serial.print(imuSensor.gz, 2);
            Serial.println(" deg/s");
            
            snprintf(msg, MSG_BUFFER_SIZE, "\r\n%lu,%lf,%lf,%lf", 
                    millis(), imuSensor.gx, imuSensor.gy, imuSensor.gz);
            appendFile(SD, FILE_GYROSCOPE, msg);

            // Log magnetometer data
            Serial.print("mx = ");
            Serial.print((int)imuSensor.mx);
            Serial.print(" my = ");
            Serial.print((int)imuSensor.my);
            Serial.print(" mz = ");
            Serial.print((int)imuSensor.mz);
            Serial.println(" mG");
            
            snprintf(msg, MSG_BUFFER_SIZE, "\r\n%lu,%lf,%lf,%lf", 
                    millis(), imuSensor.mx, imuSensor.my, imuSensor.mz);
            appendFile(SD, FILE_MAGNETOMETER, msg);

            // Log quaternion data
            Serial.print("q0 = ");
            Serial.print(*getQ());
            Serial.print(" qx = ");
            Serial.print(*(getQ() + 1));
            Serial.print(" qy = ");
            Serial.print(*(getQ() + 2));
            Serial.print(" qz = ");
            Serial.println(*(getQ() + 3));
            
            snprintf(msg, MSG_BUFFER_SIZE, "\r\n%lu,%lf,%lf,%lf,%lf", 
                    millis(), *getQ(), *(getQ() + 1), *(getQ() + 2), *(getQ() + 3));
            appendFile(SD, FILE_QUATERNION, msg);
        }

        // Calculate orientation angles
        calculateOrientation();

        if (SERIAL_DEBUG_ENABLED)
        {
            // Log orientation data
            Serial.print("Yaw, Pitch, Roll: ");
            Serial.print(imuSensor.yaw, 2);
            Serial.print(", ");
            Serial.print(imuSensor.pitch, 2);
            Serial.print(", ");
            Serial.println(imuSensor.roll, 2);

            Serial.print("Update rate = ");
            Serial.print((float)imuSensor.sumCount / imuSensor.sum, 2);
            Serial.println(" Hz");
            Serial.println();
            
            snprintf(msg, MSG_BUFFER_SIZE, "\r\n%lu,%lf,%lf,%lf,%lf", 
                    millis(), (float)imuSensor.sumCount / imuSensor.sum, imuSensor.yaw, imuSensor.pitch, imuSensor.roll);
            appendFile(SD, FILE_YPR, msg);
        }

        imuSensor.count = millis();
        imuSensor.sumCount = 0;
        imuSensor.sum = 0;

#if (PROCESSING_OUTPUT_ENABLED)
        // Output for Processing visualization
        Serial.print(imuSensor.yaw);
        Serial.print(";");
        Serial.print(imuSensor.pitch);
        Serial.print(";");
        Serial.print(imuSensor.roll);
        Serial.print(";");
        Serial.print(26.5);
        Serial.print(";");
        Serial.print(0.01);
        Serial.print(";");
        Serial.print(0.02);
        Serial.println();
#endif
    }
}
