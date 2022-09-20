#include <pgmspace.h>
#include <stdint.h>
#include "env.h"
#include "driver.h"
#include "types.h"
#include "register_module.h"
#include "resources.h"

#ifdef __cplusplus
 extern "C" {
#endif

static uint8_t menu_cursor = 0;

#define NUM_MENU 5

static const Menu nav_menu[NUM_MENU] = {
  { epd_bitmap_weather, "Weather" },
  { epd_bitmap_foreign_currency, "Currency" },
  { epd_bitmap_thermometer, "Temperature" },
  { epd_bitmap_calendar, "Calendar" },
  { epd_bitmap_settings, "Settings" }
};

static void drawMenuIcon() {
  clearDisplaySafeArea();
  LCD.setTextFont(2);
  LCD.setTextColor(TFT_BLACK, TFT_BG);
  LCD.setFreeFont(&FreeSansBold9pt7b);
  LCD.drawBitmap(55, 11, nav_menu[menu_cursor].icon, 50, 50, TFT_BLACK);
  LCD.drawString(nav_menu[menu_cursor].title, floor((TFT_W - LCD.textWidth(nav_menu[menu_cursor].title)) / 2), 61);
}

static void _init(int num, ...) {
  // Serial.println("Menu init %d\n");
  clearDisplaySafeArea();
  drawMenuIcon();
}

static void _destroy() {}

static void onKeyUp() {
   if (menu_cursor - 1 < 0) {
      menu_cursor = NUM_MENU - 1;
   } else {
      menu_cursor--;
   }
   // Serial.println(nav_menu[menu_cursor].title);
   drawMenuIcon();
}

static void onKeyDown( ) {
  menu_cursor++;
  if (menu_cursor >= NUM_MENU) {
    menu_cursor = 0;
  }
  // Serial.println(nav_menu[menu_cursor].title);
  drawMenuIcon();
}

static void onKeyRight( ) {}

static void onKeyLeft() {}

static void onKeyMid() {
  if (menu_cursor == 0) {
    ModuleSwitcher(GetModuleWeather());
    GetActiveModule().Init(0);
  } else if (menu_cursor == 2) {
    ModuleSwitcher(GetModuleTemperature());
    GetActiveModule().Init(0);
  } else if (menu_cursor == 4) {
    ModuleSwitcher(GetModuleSettings());
    GetActiveModule().Init(0);
  }
}

static void onKeySet() {}

static void onKeyReset( ) {
  menu_cursor = 0;
  GetActiveModule().Destroy();
  ModuleSwitcher(GetModuleHomescreen());
  GetActiveModule().Init(0);
}

Module menu = { _init, _destroy, onKeyUp, onKeyDown,onKeyRight, onKeyLeft, onKeyMid, onKeySet, onKeyReset };

Module GetModuleMenu() {
  return menu;
}

#ifdef __cplusplus
}
#endif
