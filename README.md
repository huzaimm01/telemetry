# Professional Vehicle Telemetry HUD

A telemetry display system designed for civilian road use, providing driving assistance through physics-based analysis and professional automotive-grade GUI, No OBDII access required.

## Features

- **Real-time Speed Display**: GPS-based speed monitoring in km/h
- **Smart Turn Analysis**: Physics-based turn radius calculation and braking recommendations
- **Live Acceleration Display**: Real-time G-force monitoring
- **Compass & Heading**: Cardinal direction display with precise degree readings
- **Drive Session Statistics**: Time, distance, and average speed tracking
- **Touch Interface**: System on/off control
- **Professional GUI**: Automotive-grade color scheme and layout

## Hardware Requirements

### Core Components
- Arduino Uno (or ESP32 for future upgrades; )
- Touchscreen (will update with model later)
- NEO-6M OR NEO-8M GPS Module
- MPU6050 IMU (Accelerometer + Gyroscope)

### Wiring Connections

#### GPS Module (NEO-6M)
```
GPS → Arduino Uno
VCC → 3.3V or 5V
GND → GND
TX  → Pin 3
RX  → Pin 4
```

#### MPU6050 IMU
```
MPU6050 → Arduino Uno
VCC → 3.3V or 5V
GND → GND
SDA → A4 (I2C Data)
SCL → A5 (I2C Clock)
```

#### TFT Shield
- Stack directly on Arduino Uno, or use jumper wires if pins conflict with GPS/IMU

## Installation

1. Clone this repository
2. Install required libraries:
   - MCUFRIEND_kbv
   - TouchScreen
   - SoftwareSerial (included with Arduino IDE)
   - Wire (included with Arduino IDE)
3. Wire hardware according to diagrams above
4. Upload code to Arduino Uno
5. Test sensors individually before vehicle installation

## File Structure

- `vehicle-hud.ino` - Main program file with setup/loop
- `sensors.ino` - GPS and IMU sensor functions
- `display.ino` - GUI drawing and update functions  
- `calculations.ino` - Physics calculations and data processing

## Safety Notice

This system is designed for **civilian road use only**. All features focus on driving awareness and safety assistance, not performance or racing applications.

## Physics Calculations

### Turn Radius Estimation
- Uses lateral acceleration from IMU: `R = v²/a`
- Recommends comfortable cornering speed (0.3G limit)
- Provides braking advisory when current speed exceeds safe cornering speed

### Distance Calculation
- Haversine formula for GPS coordinate distance calculation
- Real-time session statistics tracking

## Configuration

### Pin Modifications
If your TFT shield conflicts with GPS pins, modify these defines in `vehicle-hud.ino`:
```cpp
/* GPS PINS - Modify for your connections */
SoftwareSerial gpsSerial(4, 3);
```

### Touch Screen Calibration
Adjust touch screen mapping in `vehicle-hud.ino`:
```cpp
#define TS_MINX 150
#define TS_MINY 120  
#define TS_MAXX 920
#define TS_MAXY 940
```

## System Status Indicators

- **GPS**: Shows connection status and satellite count
- **IMU**: Displays sensor health
- **System Active**: Touch screen to toggle on/off

## Future Enhancements

- ESP32 migration for improved performance
- CAN bus integration for engine data
- Route history and analytics
- Wireless data logging

## License

MIT License - See LICENSE file for details

## Contributing

Pull requests welcome. Please maintain the professional, safety-focused approach of the original design.