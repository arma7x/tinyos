#include <YunServer.h>
#include <FileIO.h>
#include <BridgeServer.h>
#include <Bridge.h>
#include <Process.h>
#include <YunClient.h>
#include <HttpClient.h>
#include <Mailbox.h>
#include <BridgeClient.h>
#include <BridgeSSLClient.h>
#include <Console.h>
#include <BridgeUdp.h>

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

void focusChar(uint8_t);
void renderText();
void drawKeyboardUI();

static char text[26];
static uint8_t text_cursor = 0;
static uint8_t x = 0;
static uint8_t y = 0;

void renderText() {
  LCD.fillRect(2, 12, 156, 22, TFT_BG);
  LCD.setTextFont(1);
  LCD.setTextColor(TFT_BLACK, TFT_BG);
  LCD.drawString(text, 3, 13);
}

void focusChar(uint8_t f) {
  LCD.setTextFont(1);
  if (f == 0) {
    LCD.fillRect(30 + (10 * x), 36 + (11 * y), 10, 11, TFT_BLACK);
    LCD.setTextColor(TFT_BG, TFT_BLACK);
  } else {
    LCD.fillRect(30 + (10 * x), 36 + (11 * y), 10, 11, TFT_BG);
    LCD.setTextColor(TFT_BLACK, TFT_BG);
  }
  LCD.drawChar(keyboards[y][x], 30 + (10 * x) + 2, 36 + (11 * y) + 2);
}

void drawKeyboardUI() {
  LCD.fillRect(0, 10, 160, 70, TFT_BLACK);
  renderText();
  LCD.setTextFont(1);
  LCD.setTextColor(TFT_BG, TFT_BLACK);
  uint8_t x_axis=30; // +10
  uint8_t y_axis=36; // +11
  for (uint8_t y=0;y<4;y++) {
    for (uint8_t x=0;x<10;x++) {
      // LCD.fillRect(x_axis, y_axis, 10, 11, TFT_BLACK);
      LCD.drawChar(keyboards[y][x], x_axis + 2, y_axis + 2);
      x_axis += 10;
    }
    x_axis = 30;
    y_axis += 11;
  }
  focusChar(1);
}

void initKeyboardUI(int x, ...) {
  // Serial.println("Home init\n");
  clearSafeArea();
  drawKeyboardUI();
}

static void onKeyUp() {
  focusChar(0);
  if (y == 0) {
    y = 3;
  } else {
    y--;
  }
  focusChar(1);  
}

static void onKeyDown( ) {
  focusChar(0);
  if (y == 3) {
    y = 0;
  } else {
    y++;
  }
  focusChar(1);
}

static void onKeyRight( ) {
  focusChar(0);
  if (x == 9) {
    x = 0;
  } else {
    x++;
  }
  focusChar(1);
}

static void onKeyLeft() {
  focusChar(0);
  if (x == 0) {
    x = 9;
  } else {
    x--;
  }
  focusChar(1);
}

static void onKeyMid() {
  if (text_cursor == 25) {
    return;
  }
  text[text_cursor] = keyboards[y][x];
  text_cursor++;
  renderText();
}

static void onKeySet() {}

static void onKeyReset( ) {
  if (text_cursor != 0) {
    text_cursor--;
  }
  text[text_cursor] = '\0';
  renderText();  
}

static Module KeyboardUI = { initKeyboardUI, onKeyUp, onKeyDown,onKeyRight, onKeyLeft, onKeyMid, onKeySet, onKeyReset };

Module GetModuleKeyboardUI() {
  return KeyboardUI;
}

#ifdef __cplusplus
}
#endif
