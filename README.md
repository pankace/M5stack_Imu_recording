# M5Stack IMU Data Recording System

A professional IMU sensor data logger for the M5Stack platform, featuring high-precision motion tracking with SD card storage.

## Overview

This application interfaces with the MPU9250 9-axis IMU sensor to collect and log motion data from the M5Stack ESP32-based device. All sensor data is recorded to an SD card in CSV format for post-processing and analysis.

## Features

- **9-Axis Motion Tracking**: Records acceleration, gyroscope, and magnetometer data
- **AHRS Mode**: Real-time attitude estimation using Mahony quaternion filter
- **SD Card Logging**: Automatic CSV file creation and data logging
- **Visual Feedback**: LCD color indicators for system status
- **Self-Calibration**: Automatic sensor calibration on startup
- **Temperature Monitoring**: Integrated temperature sensor readings
- **Diagnostic Logging**: Self-test and calibration data for validation

## Hardware Requirements

- M5Stack Core (ESP32-based)
- MPU9250 9-axis IMU sensor (integrated in M5Stack)
- SD card (FAT32 formatted)
- USB cable for programming and power

## Sensor Data Output

The system creates the following CSV files on the SD card:

| File | Contents | Units |
|------|----------|-------|
| `acceleration.txt` | 3-axis acceleration data | milliGs (mg) |
| `gyro.txt` | 3-axis gyroscope data | degrees/second |
| `mag.txt` | 3-axis magnetometer data | milliGauss (mG) |
| `quaternion.txt` | Orientation quaternion (q0, qx, qy, qz) | unitless |
| `ypr.txt` | Yaw, Pitch, Roll angles | degrees |
| `diagnostics.txt` | Self-test and calibration data | various |

## System Operation

### Startup Sequence

1. **Blue Screen**: System initialization (30-second stabilization delay)
2. **Green Screen**: Recording active
3. **Red Screen**: Error detected (check serial output for details)

### Operating Modes

- **AHRS Mode** (default): Full attitude estimation with quaternion filtering
- **Basic Mode**: Simple sensor data output without attitude calculation

## Configuration

Key configuration parameters in [main.cpp](src/main.cpp):

```cpp
#define AHRS_MODE_ENABLED true          // Enable AHRS attitude estimation
#define SERIAL_DEBUG_ENABLED true       // Enable serial debugging output
#define STARTUP_DELAY_MS 30000          // Startup stabilization time
#define AHRS_UPDATE_INTERVAL_MS 100     // AHRS data logging rate
```

### Magnetometer Calibration

Environmental magnetic bias corrections are defined as:

```cpp
#define MAG_BIAS_X 470.0f  // X-axis bias (milliGauss)
#define MAG_BIAS_Y 120.0f  // Y-axis bias (milliGauss)
#define MAG_BIAS_Z 125.0f  // Z-axis bias (milliGauss)
```

**Note**: These values should be calibrated for your specific environment and location.

### Magnetic Declination

The default magnetic declination is set for SparkFun Electronics location:
- Location: 40°05'26.6"N, 105°11'05.9"W
- Declination: 8.5° E

Update `MAGNETIC_DECLINATION_DEG` for your location using [NOAA Magnetic Field Calculator](https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml).

## Building and Uploading

### Prerequisites

- [PlatformIO](https://platformio.org/) installed
- USB drivers for M5Stack (CH340/CP2104)

### Build Instructions

```bash
# Build the project
pio run

# Upload to M5Stack
pio run --target upload

# Open serial monitor
pio device monitor
```

### Upload Configuration

Default configuration uses COM5. To change the port, edit `platformio.ini`:

```ini
upload_port = COM[your_port_number]
monitor_port = COM[your_port_number]
```

## Serial Monitor Output

When `SERIAL_DEBUG_ENABLED` is `true`, the system outputs real-time sensor data:

```
INFO: MPU9250 Self-Test Results:
  X-axis acceleration trim: 0.5% of factory value
  ...
ax = 1005 ay = -15 az = 980 mg
gx = 0.12 gy = -0.05 gz = 0.03 deg/s
mx = 45 my = -12 mz = 38 mG
Yaw, Pitch, Roll: 175.23, 1.45, -0.87
Update rate = 9.95 Hz
```

## Code Quality

This project follows NASA C++ coding standards with emphasis on:
- Comprehensive documentation
- Clear naming conventions
- Proper error handling
- Modular function design
- Professional code formatting

## Troubleshooting

### Red Screen Error
- Check SD card is properly inserted and formatted (FAT32)
- Verify SD card has sufficient free space
- Check serial output for specific error messages

### No Data Logging
- Verify SD card is not write-protected
- Check file system integrity
- Monitor serial output for file operation errors

### Poor Orientation Accuracy
- Ensure 30-second startup stabilization completes
- Calibrate magnetometer bias values for your environment
- Keep device away from magnetic interference sources
- Update magnetic declination for your location

## Contributing

Contributions are welcome! Please ensure code follows NASA C++ standards:
- Add comprehensive documentation
- Include error handling
- Use descriptive naming
- Maintain modular structure

## License

See [LICENSE](LICENSE) file for details.

## Version History

- **v1.0** (2026-02-05): Initial release with NASA C++ standards compliance

## Acknowledgments

Built on the M5Stack platform with MPU9250 IMU sensor integration and Mahony AHRS filter implementation.

