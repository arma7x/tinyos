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
  { epd_bitmap_settings, "Settings" },
  { epd_bitmap_about, "System Info" },
};

static void drawMenu() {
  clearDisplaySafeArea();
  LCD.drawBitmap(55, 11, nav_menu[menu_cursor].icon, 50, 50, TFT_BLACK);
  LCD.setTextColor(TFT_BLACK, TFT_BG);
  LCD.setTextFont(1);
  LCD.setFreeFont(&FreeSansBold9pt7b);
  LCD.drawString(nav_menu[menu_cursor].title, floor((TFT_W - LCD.textWidth(nav_menu[menu_cursor].title)) / 2), 61);
}

static void _init(int num, ...) {
  clearDisplaySafeArea();
  drawMenu();
}

static void _destroy() {}

static void onKeyUp() {
   if (menu_cursor - 1 < 0) {
      menu_cursor = NUM_MENU - 1;
   } else {
      menu_cursor--;
   }
   drawMenu();
}

static void onKeyDown( ) {
  menu_cursor++;
  if (menu_cursor >= NUM_MENU) {
    menu_cursor = 0;
  }
  drawMenu();
}

static void onKeyRight( ) {}

static void onKeyLeft() {}

static void onKeyMid() {
  switch (menu_cursor) {
    case 0:
      ModuleSwitcher(GetModuleWeather());
      GetActiveModule().Init(0);
      break;
    case 1:
      ModuleSwitcher(GetModuleCurrency());
      GetActiveModule().Init(0);
      break;
    case 2:
      ModuleSwitcher(GetModuleTemperature());
      GetActiveModule().Init(0);
      break;
    case 3:
      ModuleSwitcher(GetModuleSettings());
      GetActiveModule().Init(1, 0);
      break;
    case 4:
      ModuleSwitcher(GetModuleSystemInfo());
      GetActiveModule().Init(0);
      break;
  }
}

static void onKeySet() {}

static void onKeyReset( ) {
  menu_cursor = 0;
  GetActiveModule().Destroy();
  ModuleSwitcher(GetModuleHomescreen());
  GetActiveModule().Init(0);
}

static Module menu = { "MENU", _init, _destroy, onKeyUp, onKeyDown,onKeyRight, onKeyLeft, onKeyMid, onKeySet, onKeyReset };

Module GetModuleMenu() {
  return menu;
}

#ifdef __cplusplus
}
#endif
