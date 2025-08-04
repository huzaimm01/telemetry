void initMPU6050() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x1C);
  Wire.write(0x00);
  Wire.endTransmission(true);
  
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x1B);
  Wire.write(0x00);
  Wire.endTransmission(true);
  
  delay(100);
  sysState.imuValid = true;
}

void readGPSData() {
  static char gpsBuffer[100];
  static uint8_t bufferIndex = 0;
  
  while (gpsSerial.available()) {
    char c = gpsSerial.read();
    
    if (c == '\n') {
      gpsBuffer[bufferIndex] = '\0';
      parseNMEA(gpsBuffer);
      bufferIndex = 0;
    } else if (bufferIndex < sizeof(gpsBuffer) - 1) {
      gpsBuffer[bufferIndex++] = c;
    }
  }
}

void parseNMEA(char* sentence) {
  if (strncmp(sentence, "$GPRMC", 6) == 0) {
    char* token = strtok(sentence, ",");
    uint8_t fieldIndex = 0;
    
    while (token != NULL && fieldIndex < 12) {
      switch (fieldIndex) {
        case 2:
          gpsData.fix = (token[0] == 'A');
          break;
        case 3:
          if (strlen(token) > 0) {
            gpsData.latitude = parseCoordinate(token);
          }
          break;
        case 5:
          if (strlen(token) > 0) {
            gpsData.longitude = parseCoordinate(token);
          }
          break;
        case 7:
          if (strlen(token) > 0) {
            gpsData.speed_kmh = atof(token) * 1.852;
          }
          break;
        case 8:
          if (strlen(token) > 0) {
            gpsData.course = atof(token);
          }
          break;
      }
      token = strtok(NULL, ",");
      fieldIndex++;
    }
    
    if (gpsData.fix) {
      gpsData.lastValidTime = millis();
      sysState.gpsValid = true;
    }
  }
}

float parseCoordinate(char* coord) {
  float degrees = atof(coord) / 100.0;
  float minutes = fmod(atof(coord), 100.0);
  return (int)degrees + minutes / 60.0;
}

void readIMUData() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU6050_ADDR, 14, true);
  
  if (Wire.available() >= 14) {
    int16_t accelX_raw = (Wire.read() << 8) | Wire.read();
    int16_t accelY_raw = (Wire.read() << 8) | Wire.read();
    int16_t accelZ_raw = (Wire.read() << 8) | Wire.read();
    
    Wire.read(); Wire.read();
    
    int16_t gyroX_raw = (Wire.read() << 8) | Wire.read();
    int16_t gyroY_raw = (Wire.read() << 8) | Wire.read();
    int16_t gyroZ_raw = (Wire.read() << 8) | Wire.read();
    
    imuData.accelX = accelX_raw / 16384.0;
    imuData.accelY = accelY_raw / 16384.0;
    imuData.accelZ = accelZ_raw / 16384.0;
    
    imuData.gyroX = gyroX_raw / 131.0;
    imuData.gyroY = gyroY_raw / 131.0;
    imuData.gyroZ = gyroZ_raw / 131.0;
    
    imuData.pitch = atan2(imuData.accelY, sqrt(imuData.accelX * imuData.accelX + imuData.accelZ * imuData.accelZ)) * 180.0 / PI;
    imuData.roll = atan2(-imuData.accelX, imuData.accelZ) * 180.0 / PI;
    
    imuData.lastReadTime = millis();
    sysState.imuValid = true;
  }
}

void handleTouch() {
  TSPoint p = ts.getPoint();
  
  if (p.z > ts.pressureThreshhold) {
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
    
    sysState.systemActive = !sysState.systemActive;
    forceRedraw = true;
    delay(200);
  }
}