#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <math.h>

MCUFRIEND_kbv tft;
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define ORANGE  0xFD20
#define GRAY    0x8410
#define DARKGRAY 0x4208

#define HUD_BG      BLACK
#define HUD_PRIMARY 0x07FF
#define HUD_ACCENT  0xFD20
#define HUD_TEXT    WHITE
#define HUD_WARNING 0xF800
#define HUD_GOOD    0x07E0
#define HUD_NEUTRAL 0x8410

/* Display (Input model***) */
#define YP A3
#define XM A2
#define YM 9
#define XP 8
#define TS_MINX 150
#define TS_MINY 120
#define TS_MAXX 920
#define TS_MAXY 940

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

/* GPS (mod later) */
SoftwareSerial gpsSerial(4, 3);

/* I2C for MPU6050 */
#define MPU6050_ADDR 0x68

struct SystemState {
  bool systemActive;
  bool gpsValid;
  bool imuValid;
  uint32_t lastUpdate;
  uint32_t sessionStartTime;
} sysState;

struct GPSData {
  float latitude;
  float longitude;
  float speed_kmh;
  float course;
  uint8_t satellites;
  bool fix;
  uint32_t lastValidTime;
} gpsData;

struct IMUData {
  float accelX, accelY, accelZ;
  float gyroX, gyroY, gyroZ;
  float pitch, roll, yaw;
  float heading;
  uint32_t lastReadTime;
} imuData;

struct DriveStats {
  float totalDistance;
  float avgSpeed;
  float maxSpeed;
  uint32_t driveTime;
  float totalAccel;
  uint16_t accelSamples;
} driveStats;

struct TurnData {
  float estimatedRadius;
  float recommendedSpeed;
  int16_t turnAngle;
  bool brakingAdvised;
  uint32_t lastTurnTime;
} turnData;

float prevLat = 0, prevLon = 0;

bool forceRedraw = true;
uint32_t lastDisplayUpdate = 0;
#define DISPLAY_UPDATE_INTERVAL 100

void setup() {
  Serial.begin(9600);
  
  uint16_t ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(1);
  tft.fillScreen(HUD_BG);
  
  gpsSerial.begin(9600);
  
  Wire.begin();
  initMPU6050();
  
  sysState.systemActive = true;
  sysState.gpsValid = false;
  sysState.imuValid = false;
  sysState.sessionStartTime = millis();
  
  memset(&gpsData, 0, sizeof(gpsData));
  memset(&imuData, 0, sizeof(imuData));
  memset(&driveStats, 0, sizeof(driveStats));
  memset(&turnData, 0, sizeof(turnData));
  
  drawHUDLayout();
  
  Serial.println("Vehicle Telemetry HUD Initialized");
}

void loop() {
  uint32_t currentTime = millis();
  
  handleTouch();
  
  if (!sysState.systemActive) {
    displaySystemOff();
    delay(100);
    return;
  }
  
  readGPSData();
  readIMUData();
  
  if (sysState.gpsValid && sysState.imuValid) {
    processTelemtryData(currentTime);
    analyzeTurnBehavior();
    updateDriveStats(currentTime);
  }
  
  if (currentTime - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL || forceRedraw) {
    updateDisplay();
    lastDisplayUpdate = currentTime;
    forceRedraw = false;
  }
}