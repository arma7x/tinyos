#include <WiFi.h>
#include <time.h>
#include <pgmspace.h>
#include <stdint.h>
#include "env.h"
#include "notification_bar.h"

#ifdef __cplusplus
 extern "C" {
#endif

TaskHandle_t SyncClockPid;
static time_t SyncClockTime;
TaskHandle_t WatchWifiConnectionPid;
static time_t WatchWifiConnectionTime;

static int calcTimezoneOffset() {
  int offset = 0;
  uint8_t idx = getPreferences().getUChar("tz", 0);
  offset += (((TZ_LIST[idx][1] - '0') * 10) + (TZ_LIST[idx][2] - '0')) * 60;
  if (strlen(TZ_LIST[idx]) == 6) {
    offset += (((TZ_LIST[idx][4] - '0') * 10) + (TZ_LIST[idx][5] - '0'));
  }
  offset *= 60;
  if (TZ_LIST[idx][0] == '-') {
    offset = -offset;
  }
  return offset;
}
void TaskSyncClock(void *pvParameters) {
  for (;;) {
    if (WiFi.getMode() == WIFI_MODE_NULL) {
      SyncClockTime = time(NULL) + 2;
      vTaskSuspend(SyncClockPid);
    }
    if (WiFi.status() == WL_CONNECTED) {
      struct tm timeinfo;
      configTime(calcTimezoneOffset(), DAYLIGHT_OFFSET_SEC, NTP_SERVER);
      if(getLocalTime(&timeinfo)) {
        SyncClockTime = time(NULL) + 2;
        vTaskSuspend(SyncClockPid);
      }
    }
    vTaskDelay(1000);
  }
}

void TaskUpdateClock(void *pvParameters) {
  for (;;) {
    time_t t = time(NULL);
    struct tm *tmp = localtime(&t);
    t = t + calcTimezoneOffset();
    drawClock((uint8_t) ((t / 3600) % 24), (uint8_t) ((t / 60) % 60), (uint8_t) (t % 60));
    vTaskDelay(1000);
  }
}

void TaskWatchWifiConnection(void *pvParameters) {
  for (;;) {
    if (WiFi.status() != WL_CONNECTED) {
      updateWifiStatus();
      WatchWifiConnectionTime = time(NULL) + 2;
      vTaskSuspend(WatchWifiConnectionPid);
    }
    vTaskDelay(5000);
  }
}

void TaskGC(void *pvParameters) {
  for (;;) {
    time_t seconds = time(NULL);
    if (SyncClockTime != NULL) {
      if (seconds > SyncClockTime) {
        vTaskDelete(SyncClockPid);
        SyncClockPid = NULL;
        SyncClockTime = NULL;
      }
    }
    if (WatchWifiConnectionTime != NULL) {
      if (seconds > WatchWifiConnectionTime) {
        vTaskDelete(WatchWifiConnectionPid);
        WatchWifiConnectionPid = NULL;
        WatchWifiConnectionTime = NULL;
      }
    }
    vTaskDelay(3000);
  }
}


#ifdef __cplusplus
}
#endif
