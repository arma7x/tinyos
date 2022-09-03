#include <pgmspace.h>
#include <stdint.h>
#include "constant.h"
#include "driver.h"
#include "module.h"
#include "register_module.h"
#include "resources.h"

#ifdef __cplusplus
 extern "C" {
#endif

uint8_t menu_cursor = 0;

typedef struct _menu {
  const unsigned char* icon;
  char* title;
} Menu;

#define NUM_MENU 5

static const Menu nav_menu[5] = {
  { epd_bitmap_weather, "Weather" },
  { epd_bitmap_foreign_currency, "Currency" },
  { epd_bitmap_temperature, "Temp" },
  { epd_bitmap_calendar, "Calendar" },
  { epd_bitmap_settings, "Settings" }
};

void clearIcon() {
  LCD.fillRect(2, 15, 50, 50, TFT_BG);
}

void clearLabel() {
  LCD.fillRect(54, 30, 106, 26, TFT_BG);
}

void drawMenuIcon() {
  clearIcon();
  clearLabel();
  LCD.setTextFont(4);
  LCD.setTextColor(TFT_BLACK, TFT_BG);
  LCD.drawString(nav_menu[menu_cursor].title, 54, 30);
  LCD.drawBitmap(2, 15, nav_menu[menu_cursor].icon, 50, 50, TFT_BLACK);
}

void initMenu(int num, ...) {
  // Serial.println("Menu init %d\n");
  clearSafeArea();
  drawMenuIcon();
}

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

static void onKeyMid() {}

static void onKeySet() {}

static void onKeyReset( ) {
  menu_cursor = 0;
  ModuleSwitcher(GetModuleHomescreen());
  GetActiveModule().Init(0);
}

Module menu = { initMenu, onKeyUp, onKeyDown,onKeyRight, onKeyLeft, onKeyMid, onKeySet, onKeyReset };

Module GetModuleMenu() {
  return menu;
}

#ifdef __cplusplus
}
#endif
