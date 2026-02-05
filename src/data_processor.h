/**
 * @file data_processor.h
 * @brief Data processing and AHRS interface
 * 
 * Provides functions for processing IMU data in both basic and AHRS modes.
 * Handles orientation calculation, data logging, and serial output.
 * 
 * @author pankace
 * @date 2026-02-05
 * @version 1.0
 */

#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

// ============================================================================
// DATA PROCESSING FUNCTIONS
// ============================================================================

/**
 * @brief Process and log basic sensor data
 * 
 * Outputs raw sensor readings to serial console for debugging.
 * Updates at BASIC_UPDATE_INTERVAL_MS rate.
 */
void processBasicMode(void);

/**
 * @brief Calculate orientation from quaternion
 * 
 * Computes Yaw, Pitch, and Roll angles from quaternion values.
 * Applies magnetic declination correction to yaw.
 */
void calculateOrientation(void);

/**
 * @brief Process and log AHRS mode data
 * 
 * Computes orientation using quaternion filter, logs all sensor data
 * to SD card, and outputs results to serial console. Updates at
 * AHRS_UPDATE_INTERVAL_MS rate.
 */
void processAHRSMode(void);

#endif // DATA_PROCESSOR_H
