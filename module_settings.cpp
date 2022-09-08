#include <WiFi.h>
#include <pgmspace.h>
#include <stdint.h>
#include "constant.h"
#include "driver.h"
#include "module.h"
#include "register_module.h"
#include "resources.h"
#include "notification_bar.h"

#ifdef __cplusplus
 extern "C" {
#endif

static void drawBrightnessLevel();
static void drawWifiStatus();

static uint8_t index_cursor = 0;

#define NUM_MENU 2

static const Menu nav_menu[NUM_MENU] = {
  { epd_bitmap_wi_fi, "Wi-Fi" },
  { epd_bitmap_sun, "Brightness" }
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
  LCD.drawString(nav_menu[index_cursor].title, 54, 30);
  LCD.drawBitmap(2, 15, nav_menu[index_cursor].icon, 50, 50, TFT_BLACK);
  if (index_cursor == 0) {
    drawWifiStatus();
  } else if (index_cursor == 1) {
    drawBrightnessLevel();
  }
}

static void drawWifiStatus() {
  LCD.fillRect(120, 30, 26, 15, TFT_BG);
  LCD.setTextFont(2);
  switch (WiFi.getMode()) {
    case WIFI_MODE_STA:
      LCD.drawString("ON", 120, 30);
      setWifiStatus(1);
      break;
    case WIFI_MODE_NULL:
      LCD.drawString("OFF", 120, 30);
      setWifiStatus(0);
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
  ledcWrite(0, brightness);
  drawBrightnessLevel();
}

static void changeWifiStatus(uint8_t value) {
  if (value == 0) {
    WiFi.mode(WIFI_MODE_NULL);
  } else {
    WiFi.mode(WIFI_MODE_STA);
  }
  drawWifiStatus();
}

static void _init(int num, ...) {
  clearSafeArea();
  drawMenuIcon();
}

static void _destroy() {}

static void onKeyUp() {
   if (index_cursor - 1 < 0) {
      index_cursor = NUM_MENU - 1;
   } else {
      index_cursor--;
   }
   drawMenuIcon();
}

static void onKeyDown( ) {
  index_cursor++;
  if (index_cursor >= NUM_MENU) {
    index_cursor = 0;
  }
  drawMenuIcon();
}

static void onKeyRight( ) {
  if (index_cursor == 0) {
    changeWifiStatus(1);
  } else if (index_cursor == 1) {
    changeLcdBrightness(1);
  }
}

static void onKeyLeft() {
  if (index_cursor == 0) {
    changeWifiStatus(0);
  } else if (index_cursor == 1) {
    changeLcdBrightness(-1);
  }
}

static void onKeyMid() {}

static void onKeySet() {}

static void onKeyReset( ) {
  index_cursor = 0;
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
