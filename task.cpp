#include <WiFi.h>
#include <time.h>
#include <pgmspace.h>
#include <stdint.h>
#include "env.h"
#include "constant.h"
#include "notification_bar.h"

#ifdef __cplusplus
 extern "C" {
#endif

TaskHandle_t syncClockPid;

void TaskSyncClock(void *pvParameters) {
  for (;;) {
    if (WiFi.getMode() == WIFI_MODE_NULL) {
      vTaskDelete(syncClockPid);
      break;
    }
    if (WiFi.status() == WL_CONNECTED) {
      struct tm timeinfo;
      configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, NTP_SERVER);
      if(getLocalTime(&timeinfo)) {
        // WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        updateWifiStatus();
        vTaskDelete(syncClockPid);
        break;
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


#ifdef __cplusplus
}
#endif
