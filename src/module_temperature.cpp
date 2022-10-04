#include <pgmspace.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "env.h"
#include "driver.h"
#include "types.h"
#include "register_module.h"
#include "resources.h"
#include <AHTxx.h>

AHTxx aht10(AHTXX_ADDRESS_X38, AHT1x_SENSOR);

#ifdef __cplusplus
 extern "C" {
#endif

TaskHandle_t TaskScanAHT10Pid;

static void taskaht10(void *pvParameters) {
  float ahtValue;
  char ahtStr[8];
  for(;;) {
    while (aht10.begin() != true) {
      Serial.println(F("AHT1x not connected or fail to load calibration coefficient"));
      vTaskDelay(5000);
    }
    ahtValue = aht10.readTemperature();
    if (ahtValue != AHTXX_ERROR) {
      LCD.setTextFont(1);
      LCD.setFreeFont(&FreeSansBold9pt7b);
      LCD.fillRect(0, 61, 80, 19, TFT_BG);
      sprintf(ahtStr, "%.2fC", ahtValue);
      LCD.drawString(ahtStr, floor((80 - LCD.textWidth(ahtStr))/ 2) , 61);
    }
    ahtValue = aht10.readHumidity();
    if (ahtValue != AHTXX_ERROR) {
      LCD.setTextFont(1);
      LCD.setFreeFont(&FreeSansBold9pt7b);
      LCD.fillRect(80, 61, 80, 19, TFT_BG);
      sprintf(ahtStr, "%.2f%%", ahtValue);
      LCD.drawString(ahtStr, floor((80 - LCD.textWidth(ahtStr))/ 2) + 80 , 61);
    }
    vTaskDelay(30000);
  }
}

static void _init(int num, ...) {
  clearDisplaySafeArea();
  LCD.drawBitmap(14, 11, epd_bitmap_thermometer2, 50, 50, TFT_BLACK);
  LCD.drawBitmap(95, 11, epd_bitmap_humidity, 50, 50, TFT_BLACK);
  if (TaskScanAHT10Pid == NULL) {
    xTaskCreatePinnedToCore(taskaht10, "taskaht10", 2048, NULL, 3, &TaskScanAHT10Pid, ARDUINO_RUNNING_CORE);
  } else {
    vTaskResume(TaskScanAHT10Pid);
  }
}

static void _destroy() {
  if (TaskScanAHT10Pid != NULL) {
    vTaskSuspend(TaskScanAHT10Pid);
    vTaskDelete(TaskScanAHT10Pid);
    TaskScanAHT10Pid = NULL;
  }
}

static void onKeyUp() {}

static void onKeyDown( ) {}

static void onKeyRight( ) {}

static void onKeyLeft() {}

static void onKeyMid() {}

static void onKeySet() {}

static void onKeyReset( ) {
  GetActiveModule().Destroy();
  ModuleSwitcher(GetModuleMenu());
  GetActiveModule().Init(0);
}

static Module temperature = { "TEMPERATURE", _init, _destroy, onKeyUp, onKeyDown,onKeyRight, onKeyLeft, onKeyMid, onKeySet, onKeyReset };

Module GetModuleTemperature() {
  return temperature;
}

#ifdef __cplusplus
}
#endif
