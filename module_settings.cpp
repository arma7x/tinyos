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


#define UTC_MENU 41
#define NUM_MENU 4

static void drawBrightnessLevel();
static void drawWifiStatus();

static uint8_t index_menu = 0;
static uint8_t index_utc = 0;

static const Menu nav_menu[NUM_MENU] = {
  { epd_bitmap_wi_fi, "Wi-Fi" },
  { epd_bitmap_sun, "Brightness" },
  { epd_bitmap_clock, "Sync Clock" },
  { epd_bitmap_globe, "Timezone" }
};

const char *utc_list[UTC_MENU] = { "+01", "+02", "+03", "+03:30", "+04", "+04:30", "+05", "+05:30",
"+05:45", "+06", "+06:30", "+07", "+08", "+08:45", "+09", "+09:30", "+10", "+10:30",
"+11", "+11:00", "+12", "+12:45", "+13", "+13:45", "+14", "-01", "-02", "-02:30",
"-03", "-03:30", "-04", "-04:30", "-05", "-06", "-07", "-08", "-09", "-09:30",
"-10", "-11", "-12" };

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
  }
}

static void drawWifiStatus() {
  LCD.fillRect(120, 30, 26, 15, TFT_BG);
  LCD.setTextFont(2);
  switch (WiFi.getMode()) {
    case WIFI_MODE_STA:
      LCD.drawString("ON", 120, 30);
      if ((WiFi.status() != WL_CONNECTED)) {
        setWifiStatus(1);
      }
      break;
    case WIFI_MODE_NULL:
      LCD.drawString("OFF", 120, 30);
      if ((WiFi.status() != WL_CONNECTED)) {
        setWifiStatus(0);
      }
      break;
  }
}

static void drawBrightnessLevel() {
  uint8_t lcd_b = getPreferences().getUChar("lcd_b", 15);
  char str[4];
  sprintf(str, "%d", lcd_b);
  LCD.fillRect(120, 30, 26, 15, TFT_BG);
  LCD.setTextFont(2);
  LCD.drawString(str, 120, 30);
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
    WiFi.mode(WIFI_MODE_NULL);
  } else {
    WiFi.mode(WIFI_MODE_STA);
  }
  drawWifiStatus();
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
  }
}

static void onKeyLeft() {
  if (index_menu == 0) {
    changeWifiStatus(0);
  } else if (index_menu == 1) {
    changeLcdBrightness(-1);
  }
}

static void onKeyMid() {
  if (index_menu == 0 && WiFi.getMode() != WIFI_MODE_NULL) {
    ModuleSwitcher(GetModuleWiFi());
    GetActiveModule().Init(0);
  } else if (index_menu == 1) {

  } else if (index_menu == 2) {
    if (SyncClockPid != NULL) {
      vTaskResume(SyncClockPid);
    } else {
      xTaskCreatePinnedToCore(TaskSyncClock, "TaskSyncClock", 2048, NULL, 3, &SyncClockPid, ARDUINO_RUNNING_CORE);
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
