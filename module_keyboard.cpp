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

typedef void(*Callback)(char*);

static void focusChar(uint8_t);
static void renderText();
static void drawKeyboardUI();
static void toggleCapsLock();
static char getCharacter(uint8_t, uint8_t);
static char resetOptions();

Callback onSetCallback;
static char text[27];
static uint8_t max_length;
static uint8_t caps_lock = 0;
static uint8_t text_cursor = 0;
static uint8_t x = 0;
static uint8_t y = 0;

static void toggleCapsLock() {
  caps_lock = caps_lock == 1 ? 0 : 1;
  drawKeyboardUI();
}

static char getCharacter(uint8_t _y, uint8_t _x) {
  char ch = keyboards[_y][_x];
  if (caps_lock == 1 && ch >= 97 && ch <= 122) {
    ch = ch - 32;
  } else if (_y == 2 && _x == 9) {
    ch = caps_lock == 1 ? 'a' : 'A';
  }
  return ch;
}

static void renderText() {
  LCD.fillRect(0, 12, 160, 22, TFT_BG);
  LCD.setTextFont(1);
  LCD.setTextColor(TFT_BLACK, TFT_BG);
  LCD.drawString(text, 2, 13);
}

static void focusChar(uint8_t f) {
  LCD.setTextFont(1);
  if (f == 0) {
    LCD.fillRect(30 + (11 * x), 36 + (11 * y), 11, 11, TFT_BLACK);
    LCD.setTextColor(TFT_BG, TFT_BLACK);
  } else {
    LCD.fillRect(30 + (11 * x), 36 + (11 * y), 11, 11, TFT_BG);
    LCD.setTextColor(TFT_BLACK, TFT_BG);
  }
  LCD.drawChar(getCharacter(y, x), 30 + (11 * x) + 3, 36 + (11 * y) + 2);
}

static void drawKeyboardUI() {
  LCD.setTextFont(1);
  LCD.setTextColor(TFT_BG, TFT_BLACK);
  uint8_t x_axis = 30; // +11
  uint8_t y_axis = 36; // +11
  for (uint8_t _y=0;_y<4;_y++) {
    for (uint8_t _x=0;_x<10;_x++) {
      LCD.drawChar(getCharacter(_y, _x), x_axis + 3, y_axis + 2);
      x_axis += 11;
    }
    x_axis = 30;
    y_axis += 11;
  }
  focusChar(1);
}

static void _init(int num, ...) {
  va_list arguments;
  va_start(arguments, num);
  for (uint8_t a=0; a<num;a++) {
    if (a == 0) {
      snprintf(text, 27, "%s", va_arg(arguments, char*));
      text_cursor = (uint8_t) strlen(text);
    } else if (a == 1) {
      // https://www.avrfreaks.net/forum/variadic-function-question-uint8t-vs-uint16t
      max_length = (uint8_t) va_arg(arguments, int);
    } else if (a == 2) {
      onSetCallback = va_arg(arguments, Callback);
    }
  }
  va_end(arguments);
  // Serial.println("Home init\n");
  clearSafeArea();
  LCD.fillRect(0, 10, 160, 70, TFT_BLACK);
  drawKeyboardUI();
  renderText();
}

static void _destroy() {
  for (uint8_t _i=0;_i<27;_i++) {
    text[_i] = '\0';  
  }
  caps_lock = 0;
  text_cursor = 0;
  x = 0;
  y = 0;
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
  if (keyboards[y][x] == 'A') {
    toggleCapsLock();
    return;
  }
  if (text_cursor == 26) {
    return;
  }
  text[text_cursor] = getCharacter(y, x);
  text_cursor++;
  renderText();
}

static void onKeySet() {
  if (onSetCallback != NULL) {
    onSetCallback(text);
  }
}

static void onKeyReset( ) {
  if (text_cursor != 0) {
    text_cursor--;
  }
  text[text_cursor] = '\0';
  renderText();  
}

static Module KeyboardUI = { _init, _destroy, onKeyUp, onKeyDown,onKeyRight, onKeyLeft, onKeyMid, onKeySet, onKeyReset };

Module GetModuleKeyboardUI() {
  return KeyboardUI;
}

#ifdef __cplusplus
}
#endif
