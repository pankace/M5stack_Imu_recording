/**
 * @file main.cpp
 * @brief M5Stack IMU Data Recording System - Main Entry Point
 * 
 * This is the main entry point for the IMU data recording system.
 * It orchestrates the initialization of all subsystems and manages
 * the main execution loop.
 * 
 * @author pankace
 * @date 2026-02-05
 * @version 1.0
 */

#include <M5Stack.h>
#include <Wire.h>
#include "config.h"
#include "sd_logger.h"
#include "imu_sensor.h"
#include "data_processor.h"
#include "utility/quaternionFilters.h"
#include "utility/MPU9250.h"

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

char msg[MSG_BUFFER_SIZE];

// ============================================================================
// SYSTEM INITIALIZATION
// ============================================================================

/**
 * @brief System initialization and setup
 * 
 * Initializes hardware peripherals, creates data files, and configures
 * all sensors for operation. The screen color indicates system status:
 * - Blue: Waiting for startup delay
 * - Green: Recording started successfully
 * - Red: Error occurred
 */
void setup(void)
{
    // Initialize communication interfaces
    Wire.begin();
    Serial.begin(115200);
    M5.begin();
    
    // Configure display
    M5.Lcd.setBrightness(SCREEN_BRIGHTNESS);
    M5.Lcd.fillScreen(BLUE);
    
    Serial.println("INFO: System starting - waiting for stabilization");
    delay(STARTUP_DELAY_MS);
    
    // Indicate recording has begun
    M5.Lcd.fillScreen(GREEN);
    Serial.println("INFO: Recording started");

    // Initialize data logging files
    initializeDataFiles();
    
    // Initialize IMU sensor
    Serial.println("INFO: MPU9250 is online");
    performIMUSelfTest();
    initializeIMU();
    
    // Initialize magnetometer
    initializeMagnetometer();
}

// ============================================================================
// MAIN PROGRAM LOOP
// ============================================================================

/**
 * @brief Main program loop
 * 
 * Continuously reads IMU data and processes it based on the selected mode:
 * - Basic mode: Simple data readout and serial output
 * - AHRS mode: Full attitude estimation with quaternion filtering
 * 
 * All data is logged to SD card files at regular intervals.
 */
void loop(void)
{
    // Log diagnostic information
    logDiagnostics();

    // Check if new data is available (data ready interrupt)
    if (imuSensor.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01)
    {
        readIMUData();
    }

    // Update quaternion filter timing
    imuSensor.updateTime();

    // Update orientation quaternion using Mahony filter
    MahonyQuaternionUpdate(imuSensor.ax, imuSensor.ay, imuSensor.az, 
                          imuSensor.gx * DEG_TO_RAD, imuSensor.gy * DEG_TO_RAD, imuSensor.gz * DEG_TO_RAD, 
                          imuSensor.my, imuSensor.mx, imuSensor.mz, imuSensor.deltat);

    // Process data based on selected mode
    if (!AHRS_MODE_ENABLED)
    {
        processBasicMode();
    }
    else
    {
        processAHRSMode();
    }
}
