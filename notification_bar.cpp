#include <WiFi.h>
#include <pgmspace.h>
#include <stdint.h>
#include "env.h"
#include "driver.h"
#include "resources.h"
#include "task.h"

#ifdef __cplusplus
 extern "C" {
#endif

void updateWifiStatus();
void setWifiStatus(bool);

void formatDigit(char* str, char n) {
  if (n < 10) {
    sprintf(str, "0%d", n);
  } else {
    sprintf(str, "%d", n);
  }
}

void drawClock(uint8_t h, uint8_t m, uint8_t s) {
  char hms[9];
  char hs[3], ms[3], ss[3];
  formatDigit(hs, h);
  formatDigit(ms, m);
  formatDigit(ss, s);
  sprintf(hms, "%s:%s:%s", hs, ms, ss);
  hms[8] = '\0';
  LCD.setTextFont(1);
  LCD.setTextColor(TFT_BLACK, TFT_BG);
  LCD.drawString(hms, 112, 1);
}

void initNotificationBar() {
  xTaskCreatePinnedToCore(TaskUpdateClock, "TaskUpdateClock", 2048, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(TaskSyncClock, "TaskSyncClock", 2048, NULL, 3, &syncClockPid, ARDUINO_RUNNING_CORE);
  updateWifiStatus();
}

void setWifiStatus(bool status) {
  LCD.setTextFont(1);
  LCD.fillRect(0, 1, 96, 10, TFT_BG);
  if (status == 1) {
    LCD.drawString("Wi-Fi:ON", 1, 1);
  } else {
    LCD.drawString("Wi-Fi:OFF", 1, 1);
  }
}

void updateWifiStatus() {
  switch (WiFi.getMode()) {
    case WIFI_MODE_STA:
      setWifiStatus(1);
      break;
    case WIFI_MODE_NULL:
      setWifiStatus(0);
      break;
  }
}


#ifdef __cplusplus
}
#endif
