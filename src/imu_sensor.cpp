/**
 * @file imu_sensor.cpp
 * @brief IMU sensor implementation
 * 
 * @author pankace
 * @date 2026-02-05
 * @version 1.0
 */

#include "imu_sensor.h"
#include "sd_logger.h"
#include "config.h"
#include <M5Stack.h>

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

MPU9250 imuSensor;
extern char msg[MSG_BUFFER_SIZE];

// ============================================================================
// IMU INITIALIZATION FUNCTIONS
// ============================================================================

void performIMUSelfTest(void)
{
    imuSensor.MPU9250SelfTest(imuSensor.SelfTest);
    
    Serial.println("INFO: MPU9250 Self-Test Results:");
    Serial.print("  X-axis acceleration trim: ");
    Serial.print(imuSensor.SelfTest[0], 1);
    Serial.println("% of factory value");
    
    Serial.print("  Y-axis acceleration trim: ");
    Serial.print(imuSensor.SelfTest[1], 1);
    Serial.println("% of factory value");
    
    Serial.print("  Z-axis acceleration trim: ");
    Serial.print(imuSensor.SelfTest[2], 1);
    Serial.println("% of factory value");
    
    Serial.print("  X-axis gyration trim: ");
    Serial.print(imuSensor.SelfTest[3], 1);
    Serial.println("% of factory value");
    
    Serial.print("  Y-axis gyration trim: ");
    Serial.print(imuSensor.SelfTest[4], 1);
    Serial.println("% of factory value");
    
    Serial.print("  Z-axis gyration trim: ");
    Serial.print(imuSensor.SelfTest[5], 1);
    Serial.println("% of factory value");
}

void initializeIMU(void)
{
    imuSensor.calibrateMPU9250(imuSensor.gyroBias, imuSensor.accelBias);
    imuSensor.initMPU9250();
    Serial.println("INFO: MPU9250 initialized for active data mode");
}

void initializeMagnetometer(void)
{
    byte deviceID = imuSensor.readByte(AK8963_ADDRESS, WHO_AM_I_AK8963);
    Serial.print("INFO: AK8963 Device ID: 0x");
    Serial.print(deviceID, HEX);
    Serial.print(" (Expected: 0x");
    Serial.print(AK8963_EXPECTED_ID, HEX);
    Serial.println(")");

    imuSensor.initAK8963(imuSensor.magCalibration);
    Serial.println("INFO: AK8963 initialized for active data mode");

    Serial.println("INFO: Magnetometer calibration values:");
    Serial.print("  X-Axis sensitivity: ");
    Serial.println(imuSensor.magCalibration[0], 2);
    Serial.print("  Y-Axis sensitivity: ");
    Serial.println(imuSensor.magCalibration[1], 2);
    Serial.print("  Z-Axis sensitivity: ");
    Serial.println(imuSensor.magCalibration[2], 2);
}

// ============================================================================
// DATA ACQUISITION FUNCTIONS
// ============================================================================

void readIMUData(void)
{
    // Read acceleration data
    imuSensor.readAccelData(imuSensor.accelCount);
    imuSensor.getAres();
    imuSensor.ax = (float)imuSensor.accelCount[0] * imuSensor.aRes;
    imuSensor.ay = (float)imuSensor.accelCount[1] * imuSensor.aRes;
    imuSensor.az = (float)imuSensor.accelCount[2] * imuSensor.aRes;

    // Read gyroscope data
    imuSensor.readGyroData(imuSensor.gyroCount);
    imuSensor.getGres();
    imuSensor.gx = (float)imuSensor.gyroCount[0] * imuSensor.gRes;
    imuSensor.gy = (float)imuSensor.gyroCount[1] * imuSensor.gRes;
    imuSensor.gz = (float)imuSensor.gyroCount[2] * imuSensor.gRes;

    // Read magnetometer data
    imuSensor.readMagData(imuSensor.magCount);
    imuSensor.getMres();
    
    // Apply magnetometer bias corrections
    imuSensor.magbias[0] = MAG_BIAS_X;
    imuSensor.magbias[1] = MAG_BIAS_Y;
    imuSensor.magbias[2] = MAG_BIAS_Z;

    // Calculate magnetometer values with calibration
    imuSensor.mx = (float)imuSensor.magCount[0] * imuSensor.mRes * imuSensor.magCalibration[0] - imuSensor.magbias[0];
    imuSensor.my = (float)imuSensor.magCount[1] * imuSensor.mRes * imuSensor.magCalibration[1] - imuSensor.magbias[1];
    imuSensor.mz = (float)imuSensor.magCount[2] * imuSensor.mRes * imuSensor.magCalibration[2] - imuSensor.magbias[2];
}

void logDiagnostics(void)
{
    snprintf(msg, MSG_BUFFER_SIZE, "\r\n%d,%lf,%lf,%lf",
        imuSensor.SelfTest[0], imuSensor.gyroBias[0], imuSensor.accelBias[0], imuSensor.magCalibration[0]);
    appendFile(SD, FILE_DIAGNOSTICS, msg);
}
