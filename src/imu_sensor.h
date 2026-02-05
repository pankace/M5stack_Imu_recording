/**
 * @file imu_sensor.h
 * @brief IMU sensor interface and initialization
 * 
 * Provides functions for initializing, calibrating, and reading data from
 * the MPU9250 IMU sensor and AK8963 magnetometer.
 * 
 * @author pankace
 * @date 2026-02-05
 * @version 1.0
 */

#ifndef IMU_SENSOR_H
#define IMU_SENSOR_H

#include "utility/MPU9250.h"

// ============================================================================
// GLOBAL IMU INSTANCE
// ============================================================================

extern MPU9250 imuSensor;

// ============================================================================
// IMU INITIALIZATION FUNCTIONS
// ============================================================================

/**
 * @brief Perform IMU self-test and report results
 * 
 * Executes the MPU9250 self-test procedure and outputs the results
 * to the serial console. Self-test values should be within 2% of
 * factory values for valid operation.
 */
void performIMUSelfTest(void);

/**
 * @brief Initialize the MPU9250 IMU sensor
 * 
 * Calibrates and initializes the accelerometer, gyroscope, and temperature
 * sensor for active data acquisition mode.
 */
void initializeIMU(void);

/**
 * @brief Initialize the AK8963 magnetometer
 * 
 * Reads device ID, retrieves factory calibration values, and initializes
 * the magnetometer for active data acquisition mode.
 */
void initializeMagnetometer(void);

// ============================================================================
// DATA ACQUISITION FUNCTIONS
// ============================================================================

/**
 * @brief Read and process IMU sensor data
 * 
 * Reads acceleration, gyroscope, and magnetometer data from the IMU.
 * Applies calibration and converts raw ADC values to engineering units.
 */
void readIMUData(void);

/**
 * @brief Log diagnostic information to SD card
 * 
 * Records self-test results, bias values, and calibration data for
 * validation and troubleshooting purposes.
 */
void logDiagnostics(void);

#endif // IMU_SENSOR_H
