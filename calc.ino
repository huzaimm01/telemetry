void processTelemtryData(uint32_t currentTime) {
  if (prevLat != 0 && prevLon != 0 && gpsData.fix) {
    float distance = calculateDistance(prevLat, prevLon, gpsData.latitude, gpsData.longitude);
    driveStats.totalDistance += distance;
  }
  
  prevLat = gpsData.latitude;
  prevLon = gpsData.longitude;
  
  if (gpsData.speed_kmh > driveStats.maxSpeed) {
    driveStats.maxSpeed = gpsData.speed_kmh;
  }
}

float calculateDistance(float lat1, float lon1, float lat2, float lon2) {
  float dLat = (lat2 - lat1) * PI / 180.0;
  float dLon = (lon2 - lon1) * PI / 180.0;
  
  float a = sin(dLat/2) * sin(dLat/2) + 
            cos(lat1 * PI / 180.0) * cos(lat2 * PI / 180.0) * 
            sin(dLon/2) * sin(dLon/2);
  
  float c = 2 * atan2(sqrt(a), sqrt(1-a));
  return 6371.0 * c;
}

void analyzeTurnBehavior() {
  float lateralAccel = sqrt(imuData.accelX * imuData.accelX + imuData.accelY * imuData.accelY);
  
  if (lateralAccel > 0.1 && gpsData.speed_kmh > 5.0) {
    float speedMs = gpsData.speed_kmh / 3.6;
    turnData.estimatedRadius = (speedMs * speedMs) / (lateralAccel * 9.81);
    
    turnData.recommendedSpeed = sqrt(0.3 * 9.81 * turnData.estimatedRadius) * 3.6;
    
    turnData.brakingAdvised = (gpsData.speed_kmh > turnData.recommendedSpeed * 1.1);
    
    turnData.turnAngle = (int16_t)(imuData.gyroZ * 2.0);
    
    turnData.lastTurnTime = millis();
  } else {
    turnData.brakingAdvised = false;
    turnData.turnAngle = 0;
  }
}

void updateDriveStats(uint32_t currentTime) {
  driveStats.driveTime = (currentTime - sysState.sessionStartTime) / 1000;
  
  if (driveStats.driveTime > 0) {
    driveStats.avgSpeed = (driveStats.totalDistance / driveStats.driveTime) * 3600.0;
  }
  
  float totalAccel = sqrt(imuData.accelX * imuData.accelX + 
                         imuData.accelY * imuData.accelY + 
                         imuData.accelZ * imuData.accelZ);
  driveStats.totalAccel += totalAccel;
  driveStats.accelSamples++;
}