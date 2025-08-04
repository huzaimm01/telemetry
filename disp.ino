void drawHUDLayout() {
  tft.fillScreen(HUD_BG);
  
  tft.fillRect(0, 0, tft.width(), 30, HUD_PRIMARY);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 8);
  tft.print("Telemetry");
  
  tft.drawRect(10, 40, 150, 80, HUD_ACCENT);
  tft.setTextColor(HUD_TEXT);
  tft.setTextSize(1);
  tft.setCursor(15, 45);
  tft.print("KMPH");
  
  tft.drawRect(170, 40, 140, 80, HUD_ACCENT);
  tft.setCursor(175, 45);
  tft.print("CORNER ASSIST");
  
  tft.drawRect(10, 130, 100, 60, HUD_ACCENT);
  tft.setCursor(15, 135);
  tft.print("FORCES");
  
  tft.drawRect(120, 130, 90, 60, HUD_ACCENT);
  tft.setCursor(125, 135);
  tft.print("HEADING");
  
  tft.drawRect(220, 130, 90, 60, HUD_ACCENT);
  tft.setCursor(225, 135);
  tft.print("SESSION");
  
  tft.drawRect(10, 200, 300, 30, HUD_NEUTRAL);
  tft.setCursor(15, 208);
  tft.print("GPS: -- | IMU: -- | SATS: --");
}

void updateDisplay() {
  tft.fillRect(15, 55, 140, 60, HUD_BG);
  tft.setTextColor(HUD_PRIMARY);
  tft.setTextSize(4);
  tft.setCursor(20, 70);
  if (sysState.gpsValid && gpsData.fix) {
    tft.print((int)gpsData.speed_kmh);
  } else {
    tft.print("---");
  }
  
  tft.fillRect(175, 55, 130, 60, HUD_BG);
  tft.setTextSize(2);
  if (turnData.brakingAdvised) {
    tft.setTextColor(HUD_WARNING);
    tft.setCursor(180, 65);
    tft.print("BRAKE");
    tft.setCursor(180, 85);
    tft.print((int)turnData.recommendedSpeed);
    tft.print(" KM/H");
  } else if (abs(turnData.turnAngle) > 5) {
    tft.setTextColor(HUD_ACCENT);
    tft.setCursor(180, 65);
    tft.print("TURN ");
    tft.print(turnData.turnAngle);
    tft.print("°");
  } else {
    tft.setTextColor(HUD_GOOD);
    tft.setCursor(195, 75);
    tft.print("OK");
  }
  
  tft.fillRect(15, 145, 90, 40, HUD_BG);
  tft.setTextSize(2);
  tft.setTextColor(HUD_TEXT);
  tft.setCursor(20, 155);
  if (sysState.imuValid) {
    float totalG = sqrt(imuData.accelX * imuData.accelX + 
                       imuData.accelY * imuData.accelY + 
                       imuData.accelZ * imuData.accelZ);
    tft.print(totalG, 1);
  } else {
    tft.print("--");
  }
  
  tft.fillRect(125, 145, 80, 40, HUD_BG);
  tft.setTextSize(1);
  tft.setTextColor(HUD_TEXT);
  tft.setCursor(130, 150);
  if (sysState.gpsValid && gpsData.fix) {
    tft.print(getCardinalDirection(gpsData.course));
    tft.setCursor(130, 165);
    tft.print((int)gpsData.course);
    tft.print("°");
  } else {
    tft.print("---");
  }
  
  tft.fillRect(225, 145, 80, 40, HUD_BG);
  tft.setTextSize(1);
  tft.setTextColor(HUD_TEXT);
  tft.setCursor(230, 150);
  tft.print(driveStats.driveTime / 60);
  tft.print(":");
  tft.print(driveStats.driveTime % 60);
  tft.setCursor(230, 160);
  tft.print(driveStats.totalDistance, 1);
  tft.print("km");
  tft.setCursor(230, 170);
  tft.print((int)driveStats.avgSpeed);
  tft.print("avg");
  
  tft.fillRect(15, 205, 290, 20, HUD_BG);
  tft.setTextSize(1);
  tft.setTextColor(HUD_TEXT);
  tft.setCursor(15, 208);
  tft.print("GPS: ");
  tft.print(sysState.gpsValid ? "OK" : "NO");
  tft.print(" | IMU: ");
  tft.print(sysState.imuValid ? "OK" : "NO");
  tft.print(" | SATS: ");
  tft.print(gpsData.satellites);
}

const char* getCardinalDirection(float heading) {
  int index = (int)((heading + 11.25) / 22.5) % 16;
  const char* directions[] = {"N", "NE", "E", "SE",
                             "S", "SW", "W", "NW"};
  return directions[index];
}

void displaySystemOff() {
  tft.fillScreen(HUD_BG);
  tft.setTextColor(HUD_NEUTRAL);
  tft.setTextSize(3);
  tft.setCursor(80, 100);
  tft.print("SYSTEM OFF");
  tft.setTextSize(1);
  tft.setCursor(100, 140);
  tft.print("Touch to activate");
}