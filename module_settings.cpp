#include <WiFi.h>
#include <pgmspace.h>
#include <stdint.h>
#include "env.h"
#include "driver.h"
#include "types.h"
#include "register_module.h"
#include "resources.h"
#include "notification_bar.h"
#include "task.h"

#ifdef __cplusplus
 extern "C" {
#endif


#define NUM_MENU 4

static void drawBrightnessLevel();
static void drawWifiStatus();
static void drawTimezone();

static uint8_t index_menu = 0;
static uint8_t index_tz = 0;

static const Menu nav_menu[NUM_MENU] = {
  { epd_bitmap_wi_fi, "Wi-Fi" },
  { epd_bitmap_sun, "Brightness" },
  { epd_bitmap_clock, "Sync Clock" },
  { epd_bitmap_globe, "Timezone" }
};

static void clearIcon() {
  LCD.fillRect(2, 15, 50, 50, TFT_BG);
}

static void clearLabel() {
  LCD.fillRect(54, 30, 106, 26, TFT_BG);
  LCD.setTextFont(1);
  LCD.setFreeFont(&FreeSansBold9pt7b);
  LCD.drawString("Settings", 66, 12);
}

static void drawMenuIcon() {
  clearIcon();
  clearLabel();
  LCD.setTextFont(2);
  LCD.setTextColor(TFT_BLACK, TFT_BG);
  LCD.drawString(nav_menu[index_menu].title, 54, 30);
  LCD.drawBitmap(2, 15, nav_menu[index_menu].icon, 50, 50, TFT_BLACK);
  if (index_menu == 0) {
    drawWifiStatus();
  } else if (index_menu == 1) {
    drawBrightnessLevel();
  } else if (index_menu == 2) {

  } else if (index_menu == 3) {
    drawTimezone();
  }
}

static void drawWifiStatus() {
  LCD.setTextFont(2);
  LCD.fillRect(TFT_W - LCD.textWidth(OFF) - 2, 30, LCD.textWidth(OFF) + 2, 15, TFT_BG);
  switch (WiFi.getMode()) {
    case WIFI_MODE_STA:
      LCD.drawString(ON, TFT_W - LCD.textWidth(ON) - 2, 30);
      if ((WiFi.status() != WL_CONNECTED)) {
        setWifiStatus(1);
      }
      break;
    case WIFI_MODE_NULL:
      LCD.drawString(OFF, TFT_W - LCD.textWidth(OFF) - 2, 30);
      if ((WiFi.status() != WL_CONNECTED)) {
        setWifiStatus(0);
      }
      break;
  }
}

static void drawBrightnessLevel() {
  uint8_t lcd_b = getPreferences().getUChar("lcd_b", 0);
  char str[4];
  sprintf(str, "%d", lcd_b);
  LCD.setTextFont(2);
  LCD.fillRect(TFT_W - LCD.textWidth("255") - 2, 30, LCD.textWidth("255") + 2, 15, TFT_BG);
  LCD.drawString(str, TFT_W - LCD.textWidth(str) - 2, 30);
}

static void drawTimezone() {
  index_tz = getPreferences().getUChar("tz", 0);
  LCD.setTextFont(2);
  LCD.fillRect(TFT_W - 41 - 2, 30, 41 + 2, 15, TFT_BG);
  LCD.drawString(TZ_LIST[index_tz], TFT_W - LCD.textWidth(TZ_LIST[index_tz]) - 2, 30);
}

static void changeLcdBrightness(uint8_t value) {
  uint8_t brightness = getPreferences().getUChar("lcd_b", 15);
  if (brightness >= 255 && value == 1) {
    return;
  }
  brightness += value * 5;
  if (brightness <= 0) {
    brightness = 5;
  }
  getPreferences().putUChar("lcd_b", brightness);
  setDisplayBrightness(brightness);
  drawBrightnessLevel();
}

static void changeWifiStatus(bool value) {
  if (value == 0) {
    if ((WiFi.status() == WL_CONNECTED)) {
      WiFi.disconnect(true);
    }
    if (ConnectToWifiPid != NULL) {
      vTaskDelete(ConnectToWifiPid);
      ConnectToWifiPid = NULL;
      ConnectToWifiTime = NULL;
    }
    WiFi.mode(WIFI_MODE_NULL);
  } else {
    WiFi.mode(WIFI_MODE_STA);
  }
  drawWifiStatus();
}

static void changeTimezone(int8_t i) {
  if (index_tz + i > -1 && index_tz + i < TZ_NUM) {
    index_tz += i;
    getPreferences().putUChar("tz", index_tz);
    drawTimezone();
  }
}

static void _init(int num, ...) {
  clearDisplaySafeArea();
  drawMenuIcon();
}

static void _destroy() {}

static void onKeyUp() {
   if (index_menu - 1 < 0) {
      index_menu = NUM_MENU - 1;
   } else {
      index_menu--;
   }
   drawMenuIcon();
}

static void onKeyDown( ) {
  index_menu++;
  if (index_menu >= NUM_MENU) {
    index_menu = 0;
  }
  drawMenuIcon();
}

static void onKeyRight( ) {
  if (index_menu == 0) {
    changeWifiStatus(1);
  } else if (index_menu == 1) {
    changeLcdBrightness(1);
  } else if (index_menu == 2) {
   //
  } else if (index_menu == 3) {
    changeTimezone(1);
  }
}

static void onKeyLeft() {
  if (index_menu == 0) {
    changeWifiStatus(0);
  } else if (index_menu == 1) {
    changeLcdBrightness(-1);
  } else if (index_menu == 2) {
   //
  } else if (index_menu == 3) {
    changeTimezone(-1);
  }
}

static void onKeyMid() {
  if (index_menu == 0 && WiFi.getMode() != WIFI_MODE_NULL) {
    ModuleSwitcher(GetModuleWiFi());
    GetActiveModule().Init(0);
  } else if (index_menu == 1) {

  } else if (index_menu == 2) {
    if (SyncClockPid == NULL) { 
      xTaskCreatePinnedToCore(TaskSyncClock, "TaskSyncClock", 2048, NULL, 3, &SyncClockPid, ARDUINO_RUNNING_CORE);
    } else {
      vTaskResume(SyncClockPid);
    }
  }
}

static void onKeySet() {}

static void onKeyReset( ) {
  index_menu = 0;
  GetActiveModule().Destroy();
  ModuleSwitcher(GetModuleMenu());
  GetActiveModule().Init(0);
}

Module settings = { _init, _destroy, onKeyUp, onKeyDown,onKeyRight, onKeyLeft, onKeyMid, onKeySet, onKeyReset };

Module GetModuleSettings() {
  return settings;
}

#ifdef __cplusplus
}
#endif
