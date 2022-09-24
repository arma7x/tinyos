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

void drawHomescreen() {
  LCD.setTextFont(1);
  // LCD.drawBitmap(45, 10, wallpaper, 70, 70, TFT_BLACK);
  uint8_t w_w = LCD.textWidth("Wi-Fi");
  uint8_t m_w = LCD.textWidth("Menu");
  uint8_t s_w = LCD.textWidth("Settings");
  LCD.drawBitmap(floor((w_w - 12) / 2) + 1, TFT_H - 22, epd_bitmap_wifi_small, 12, 12, TFT_BLACK);
  LCD.drawString("Wi-Fi", 1, TFT_H - 9);
  LCD.drawBitmap(floor((TFT_W - 12) / 2), TFT_H - 22, epd_bitmap_menu_small, 12, 12, TFT_BLACK);
  LCD.drawString("Menu", floor((TFT_W - m_w) / 2), TFT_H - 9);
  LCD.drawBitmap(floor(TFT_W - (s_w + 12) / 2) - 1, TFT_H - 22, epd_bitmap_gear_small, 12, 12, TFT_BLACK);
  LCD.drawString("Settings", TFT_W - s_w - 1, TFT_H - 9);
}

static void _init(int num, ...) {
  // Serial.println("Home init\n");
  clearDisplaySafeArea();
  drawHomescreen();
}

static void _destroy() {}

static void onKeyUp() {}

static void onKeyDown( ) {}

static void onKeyRight( ) {}

static void onKeyLeft() {}

static void onKeyMid() {
  GetActiveModule().Destroy();
  ModuleSwitcher(GetModuleMenu());
  GetActiveModule().Init(0);
}

static void onKeySet() {}

static void onKeyReset( ) {}

static Module homescreen = { "HOMESCREEN", _init, _destroy, onKeyUp, onKeyDown,onKeyRight, onKeyLeft, onKeyMid, onKeySet, onKeyReset };

Module GetModuleHomescreen() {
  return homescreen;
}

#ifdef __cplusplus
}
#endif
