# Calibration Guide

Proper calibration is essential for accurate IMU measurements. This guide covers magnetometer calibration and magnetic declination setup.

## Magnetometer Calibration

The magnetometer measures Earth's magnetic field, but local magnetic interference (hard iron and soft iron distortions) affects readings. Calibration corrects these errors.

### When to Calibrate

- First time setup
- After relocating the device to a new environment
- When near large metal objects or magnetic sources
- If yaw/heading readings drift or are inaccurate

### Calibration Procedure

#### 1. Prepare the Environment

- Move away from:
  - Large metal objects (filing cabinets, cars, machinery)
  - Electronic devices (computers, phones, motors)
  - Magnetic sources (magnets, speakers, power supplies)
  - Steel-reinforced concrete buildings (if possible)

#### 2. Collect Calibration Data

**Method A: Figure-8 Pattern (Recommended)**
1. Hold the M5Stack level (screen facing up)
2. Slowly move it in a figure-8 pattern for 30 seconds
3. Keep movements smooth and cover all orientations
4. Repeat with device vertical (screen facing forward)

**Method B: Sphere Coverage**
1. Slowly rotate the device around all three axes
2. Ensure you cover points on an imaginary sphere
3. Continue for at least 60 seconds
4. Move smoothly without sudden jerks

#### 3. Record Min/Max Values

While performing calibration movements, record the minimum and maximum values for each magnetometer axis from the serial output:

```
mx = 45 my = -12 mz = 38 mG
mx = 52 my = -8 mz = 42 mG
mx = 38 my = -15 mz = 35 mG
...
```

Record:
- X-axis: min = ____, max = ____
- Y-axis: min = ____, max = ____
- Z-axis: min = ____, max = ____

#### 4. Calculate Bias Corrections

For each axis, calculate the bias:

```
bias = (max + min) / 2
```

Example:
```
X: max = 520 mG, min = 420 mG → bias = (520 + 420) / 2 = 470 mG
Y: max = 130 mG, min = 110 mG → bias = (130 + 110) / 2 = 120 mG
Z: max = 140 mG, min = 110 mG → bias = (140 + 110) / 2 = 125 mG
```

#### 5. Update Code

Edit [main.cpp](src/main.cpp) and update the bias values:

```cpp
// Magnetometer bias corrections (milliGauss)
#define MAG_BIAS_X 470.0f  // Your calculated X bias
#define MAG_BIAS_Y 120.0f  // Your calculated Y bias
#define MAG_BIAS_Z 125.0f  // Your calculated Z bias
```

#### 6. Verify Calibration

After uploading the updated code:
1. Rotate the device slowly through all orientations
2. Watch the magnetometer readings in the serial monitor
3. Values should be centered around zero after bias correction
4. Yaw angle should be stable and accurate

### Advanced: Scale Factor Calibration

For better accuracy, also calculate scale factors:

```
scale = (max - min) / 2
```

The scale factors for all three axes should be similar (within 10%). If they differ significantly, you may have soft iron distortion that requires a more complex calibration matrix.

## Magnetic Declination

Magnetic declination is the angle between magnetic north (compass) and true north (geographic). This varies by location and must be corrected for accurate heading.

### Finding Your Declination

**Option 1: NOAA Calculator (Most Accurate)**
1. Visit: https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml
2. Enter your latitude and longitude
3. Click "Calculate"
4. Note the "Declination" value (in degrees)

**Option 2: Smartphone Compass Apps**
- Many compass apps show both true and magnetic heading
- The difference is your declination

**Option 3: Topographic Maps**
- Declination is often printed on topo maps
- Note: Values change over time (secular variation)

### Update Code with Declination

Edit [main.cpp](src/main.cpp):

```cpp
// Magnetic declination for location (degrees)
#define MAGNETIC_DECLINATION_DEG 8.5f  // Replace with your value
```

**Important Notes:**
- East declination: positive value (e.g., +8.5°)
- West declination: negative value (e.g., -8.5°)
- Declination changes slowly over time (~0.1-0.2° per year)
- Update annually for best accuracy

### Declination by Region (Approximate)

These are rough estimates for 2026. Always verify for your specific location:

| Region | Approximate Declination |
|--------|------------------------|
| US West Coast | +10° to +15° E |
| US East Coast | -10° to -15° W |
| Central US | -5° to +5° |
| UK | -2° to +2° |
| Western Europe | -2° to +2° |
| Eastern Europe | +5° to +10° E |
| Australia (East) | +10° to +12° E |
| Australia (West) | -2° to +2° |
| Japan | -5° to -8° W |

## Accelerometer and Gyroscope Calibration

The MPU9250 automatically performs self-calibration for the accelerometer and gyroscope during the startup sequence.

### Startup Calibration Requirements

For accurate calibration:
1. Place device on a **level, stable surface**
2. Do **not move** during the 30-second startup delay
3. Keep away from vibration sources
4. Wait for the **green screen** before moving

### Verifying Self-Test Results

The serial output shows self-test results:

```
INFO: MPU9250 Self-Test Results:
  X-axis acceleration trim: 0.5% of factory value
  Y-axis acceleration trim: 0.8% of factory value
  Z-axis acceleration trim: 0.6% of factory value
  X-axis gyration trim: 1.2% of factory value
  Y-axis gyration trim: 0.9% of factory value
  Z-axis gyration trim: 1.1% of factory value
```

**Good Results:**
- All values < 2%: Excellent calibration
- All values < 5%: Acceptable calibration

**Poor Results:**
- Any value > 5%: Repeat calibration
- Any value > 10%: Sensor may be damaged

If results are consistently poor:
1. Ensure device is completely still during startup
2. Check for mechanical damage
3. Try power cycling the device
4. Verify stable power supply

## Temperature Effects

The IMU sensors are temperature-sensitive. For best results:
- Allow device to reach operating temperature before calibration
- Calibrate in the same environment where you'll use it
- Recalibrate if moving between significantly different temperatures

## Troubleshooting

### Magnetometer readings are unstable
- Recalibrate in a magnetically clean environment
- Check for nearby magnetic interference sources
- Ensure proper bias correction is applied

### Yaw angle drifts over time
- Verify magnetic declination is correct
- Check for local magnetic anomalies
- Magnetometer may need recalibration
- Soft iron distortion may be present

### Accelerometer/gyroscope drift
- Ensure device is still during startup calibration
- Check mounting - device must be rigid
- Temperature changes can cause drift
- Self-test values should be < 2%

### Inconsistent readings
- Verify stable power supply
- Check for loose connections
- Ensure SD card operations aren't causing power fluctuations
- Add delays between rapid sensor reads

## Data Validation

After calibration, validate your setup:

1. **Static Test**: Device stationary should show:
   - Acceleration: ~1000 mg on Z-axis, ~0 on X and Y
   - Gyroscope: All axes near 0 deg/s
   - Magnetometer: Stable readings

2. **Rotation Test**: Slow 360° rotation should show:
   - Smooth yaw angle change from 0° to 360°
   - Pitch and roll remain stable
   - No sudden jumps or discontinuities

3. **Tilt Test**: Tilt to various orientations:
   - Pitch and roll should match physical angle
   - Quaternion should remain normalized
   - No gimbal lock at ±90° pitch

## Calibration Log Template

Keep a calibration log for reference:

```
Date: _______________
Location: _______________
Temperature: _______________

Magnetometer:
  X-axis: min = _____ mG, max = _____ mG, bias = _____ mG
  Y-axis: min = _____ mG, max = _____ mG, bias = _____ mG
  Z-axis: min = _____ mG, max = _____ mG, bias = _____ mG

Magnetic Declination: _____ ° (E/W)

Self-Test Results:
  Accel X: _____%  Accel Y: _____%  Accel Z: _____%
  Gyro X:  _____%  Gyro Y:  _____%  Gyro Z:  _____%

Notes:
____________________________________
____________________________________
```

## References

- [NOAA Magnetic Field Calculator](https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml)
- [MPU9250 Product Specification](https://invensense.tdk.com/products/motion-tracking/9-axis/mpu-9250/)
- [Magnetometer Calibration Theory](https://www.nxp.com/docs/en/application-note/AN4246.pdf)
