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

const char *MONTHS[12] PROGMEM = {"JAN", "FEB", "MAC", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC"};
const char *DAYS[7] PROGMEM = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

void updateWifiStatus();
void setWifiStatus(bool);

void formatDigit(char* str, char n) {
  if (n < 10) {
    sprintf(str, "0%d", n);
  } else {
    sprintf(str, "%d", n);
  }
}

void drawClockWidget(struct tm* tm) {
  char hms[9];
  sprintf(hms, "%02d:%02d:%02d", (*tm).tm_hour, (*tm).tm_min, (*tm).tm_sec);
  hms[8] = '\0';
  LCD.setFreeFont(&FreeSans9pt7b);
  LCD.setTextFont(1);
  LCD.setTextColor(TFT_BLACK, TFT_BG);
  LCD.drawString(hms, 112, 1);
}

void drawHomescreenWidget(struct tm* tm) {
  LCD.setFreeFont(&FreeSans9pt7b);
  LCD.setTextFont(1);
  char tpl[17];
  uint8_t t_w = LCD.textWidth("THU, 01 JAN 1970");
  sprintf(tpl, "%s, %02d %s %d", DAYS[(*tm).tm_wday], (*tm).tm_mday, MONTHS[(*tm).tm_mon], (*tm).tm_year + 1900);
  LCD.fillRect(floor((TFT_W - t_w) / 2), 12, t_w, 9, TFT_BG);
  LCD.drawString(tpl, floor((TFT_W - t_w) / 2), 12);
  memset(&tpl[0], 0, sizeof(tpl));
  LCD.setTextFont(1);
  LCD.setFreeFont(&FreeMonoBold12pt7b);
  uint8_t b_w = LCD.textWidth("99:99");
  sprintf(tpl, "%02d:%02d", (*tm).tm_hour, (*tm).tm_min);
  LCD.fillRect(floor((TFT_W - b_w) / 2), 21, b_w, 20, TFT_BG);
  LCD.drawString(tpl, floor((TFT_W - b_w) / 2), 21);
  LCD.setFreeFont(&FreeSans9pt7b);
  LCD.setTextFont(1);
}

void initNotificationBar() {
  xTaskCreatePinnedToCore(TaskUpdateClock, "TaskUpdateClock", 2048, NULL, 3, NULL, ARDUINO_RUNNING_CORE);
  xTaskCreatePinnedToCore(TaskSyncClock, "TaskSyncClock", 2048, NULL, 3, &SyncClockPid, ARDUINO_RUNNING_CORE);
  updateWifiStatus();
}

void setWifiStatus(bool status) {
  LCD.setFreeFont(&FreeSans9pt7b);
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
