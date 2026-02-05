/**
 * @file sd_logger.h
 * @brief SD card logging interface
 * 
 * Provides functions for writing and appending data to files on the SD card.
 * Handles file creation, error reporting, and CSV header initialization.
 * 
 * @author pankace
 * @date 2026-02-05
 * @version 1.0
 */

#ifndef SD_LOGGER_H
#define SD_LOGGER_H

#include <M5Stack.h>

// ============================================================================
// SD CARD FILE OPERATIONS
// ============================================================================

/**
 * @brief Append data to a file on the SD card
 * 
 * @param fs File system reference
 * @param path Path to the file
 * @param message Message to append
 * 
 * @note On failure, the LCD screen will turn red to indicate an error
 */
void appendFile(fs::FS& fs, const char* path, const char* message);

/**
 * @brief Write data to a new file on the SD card
 * 
 * @param fs File system reference
 * @param path Path to the file
 * @param message Message to write
 * 
 * @note On failure, the LCD screen will turn red to indicate an error
 */
void writeFile(fs::FS& fs, const char* path, const char* message);

/**
 * @brief Initialize data logging files on SD card
 * 
 * Creates CSV files with headers for all sensor data types:
 * - Acceleration data
 * - Gyroscope data
 * - Magnetometer data
 * - Quaternion orientation data
 * - Yaw/Pitch/Roll data
 * - Diagnostic information
 */
void initializeDataFiles(void);

#endif // SD_LOGGER_H
