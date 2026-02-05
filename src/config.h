/**
 * @file config.h
 * @brief Configuration constants for M5Stack IMU Data Recording System
 * 
 * This file contains all configuration parameters, calibration values,
 * and compile-time constants used throughout the application.
 * 
 * @author pankace
 * @date 2026-02-05
 * @version 1.0
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// FEATURE CONFIGURATION
// ============================================================================

#define PROCESSING_OUTPUT_ENABLED false
#define AHRS_MODE_ENABLED true
#define SERIAL_DEBUG_ENABLED true

// ============================================================================
// TIMING CONSTANTS (milliseconds)
// ============================================================================

#define STARTUP_DELAY_MS 30000
#define AHRS_UPDATE_INTERVAL_MS 100
#define BASIC_UPDATE_INTERVAL_MS 500

// ============================================================================
// DISPLAY CONFIGURATION
// ============================================================================

#define SCREEN_BRIGHTNESS 200

// ============================================================================
// BUFFER SIZES
// ============================================================================

#define MSG_BUFFER_SIZE 100

// ============================================================================
// MAGNETOMETER CALIBRATION
// ============================================================================

// Environmental magnetic bias corrections (milliGauss)
// These values should be calibrated for your specific environment
#define MAG_BIAS_X 470.0f
#define MAG_BIAS_Y 120.0f
#define MAG_BIAS_Z 125.0f

// ============================================================================
// TEMPERATURE CONVERSION
// ============================================================================

#define TEMP_CONVERSION_FACTOR 333.87f
#define TEMP_OFFSET 21.0f

// ============================================================================
// LOCATION SETTINGS
// ============================================================================

// Magnetic declination for location (degrees)
// Default: SparkFun Electronics (40°05'26.6"N 105°11'05.9"W) = 8.5° E
// Update this value for your specific location using:
// https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml
#define MAGNETIC_DECLINATION_DEG 8.5f

// ============================================================================
// DEVICE IDENTIFICATION
// ============================================================================

#define AK8963_EXPECTED_ID 0x48

// ============================================================================
// FILE PATHS
// ============================================================================

#define FILE_ACCELERATION "/acceleration.txt"
#define FILE_GYROSCOPE "/gyro.txt"
#define FILE_MAGNETOMETER "/mag.txt"
#define FILE_QUATERNION "/quaternion.txt"
#define FILE_YPR "/ypr.txt"
#define FILE_DIAGNOSTICS "/diagnostics.txt"

#endif // CONFIG_H
