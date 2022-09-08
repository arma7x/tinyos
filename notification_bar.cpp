#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#include <WiFi.h>
#include <time.h>
#include <pgmspace.h>
#include <stdint.h>
#include "env.h"
#include "constant.h"
#include "driver.h"
#include "resources.h"

#ifdef __cplusplus
 extern "C" {
#endif

TaskHandle_t syncClock;

void updateWifiStatus();
void setWifiStatus(uint8_t);

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

void TaskSyncClock(void *pvParameters) {
  for (;;) {
    if (WiFi.status() == WL_CONNECTED) {
      struct tm timeinfo;
      if(getLocalTime(&timeinfo)) {
        // WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        updateWifiStatus();
        vTaskDelete(syncClock);
      }
    }
    vTaskDelay(1000);
  }
}

void TaskUpdateClock(void *pvParameters) {
  for (;;) {
    time_t t = time(NULL);
    struct tm *tmp = localtime(&t);
    t = t + GMT_OFFSET_SEC;
    drawClock((uint8_t) ((t / 3600) % 24), (uint8_t) ((t / 60) % 60), (uint8_t) (t % 60));
    vTaskDelay(1000);
  }
}

void InitNotificationBar() {
  xTaskCreatePinnedToCore(
    TaskUpdateClock,  
    "TaskUpdateClock",   // A name just for humans
    1024,  // This stack size can be checked & adjusted by reading the Stack Highwater
    NULL,
    3,  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    NULL, 
    ARDUINO_RUNNING_CORE
  );
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
  xTaskCreate(TaskSyncClock, "TaskSyncClock", 1024, NULL,3, &syncClock);
  updateWifiStatus();
}

void setWifiStatus(uint8_t status) {
  LCD.setTextFont(1);
  LCD.fillRect(0, 1, 54, 10, TFT_BG);
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
