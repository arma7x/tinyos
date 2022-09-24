#include <pgmspace.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "env.h"
#include "driver.h"
#include "types.h"
#include "register_module.h"
#include "resources.h"

#ifdef __cplusplus
 extern "C" {
#endif

#define KEYS 19
#define COLUMNS 5

const char *weather_keys[] PROGMEM = {"?", "mm", "=", "///", "//", "**", "*/*", "/", ".", "x", "x/", "*", "*/", "m", "o", "/!/", "!/", "*!*", "mmm"};

const char *weather_icons[KEYS][COLUMNS] PROGMEM = {
  {"    .-.      ","     __)     ","    (        ","     `-’     ","      •      "},
  {"             ","     .--.    ","  .-(    ).  "," (___.__)__) ","             "},
  {"             "," _ - _ - _ - ","  _ - _ - _  "," _ - _ - _ - ","             "},
  {"     .-.     ","    (   ).   ","   (___(__)  ","  ‚'‚'‚'‚'   ","  ‚’‚’‚’‚’   "},
  {" _`/\"\".-.    ","  ,\\_(   ).  ","   /(___(__) ","   ‚'‚'‚'‚'  ","   ‚’‚’‚’‚’  "},
  {"     .-.     ","    (   ).   ","   (___(__)  ","   * * * *   ","  * * * *    "},
  {" _`/\"\".-.    ","  ,\\_(   ).  ","   /(___(__) ","    * * * *  ","   * * * *   "},
  {"     .-.     ","    (   ).   ","   (___(__)  ","    ' ' ' '  ","   ' ' ' '   "},
  {" _`/\"\".-.    ","  ,\\_(   ).  ","   /(___(__) ","     ' ' ' ' ","    ' ' ' '  "},
  {"     .-.     ","    (   ).   ","   (___(__)  ","    ' * ' *  ","   * ' * '   "},
  {" _`/\"\".-.    ","  ,\\_(   ).  ","   /(___(__) ","     ' * ' * ","    * ' * '  "},
  {"     .-.     ","    (   ).   ","   (___(__)  ","    *  *  *  ","   *  *  *   "},
  {" _`/\"\".-.    ","  ,\\_(   ).  ","   /(___(__) ","     *  *  * ","    *  *  *  "},
  {"   \\  /      "," _ /\"\".-.    ","   \\_(   ).  ","   /(___(__) ","             "},
  {"    \\   /   ","     .-.     ","  ― (   ) ―  ","     `-’     ","    /   \\    "},
  {"     .-.     ","    (   ).   ","   (___(__)  ","  ‚''‚‚' ","  ‚’‚’’‚’  "},
  {" _`/\"\".-.    ","  ,\\_(   ). ","   /(___(__) ","    /' '' '  ","    ' ' ' '  "},
  {" _`/\"\".-.    ","  ,\\_(   ).  ","   /(___(__) ","     ***     ","    *  *  *  "},
  {"             ","     .--.    ","  .-(    ).  "," (___.__)__) ","             "},
};

uint8_t findIndex(const char *key) {
  uint8_t x=0;
  for (x=0;x<KEYS;x++) {
    if (strcmp(weather_keys[x], key) == 0) {
      break;
    }
  }
  if (x >= KEYS) {
    x = 0;
  }
  return x;
}

static void _init(int num, ...) {
  clearDisplaySafeArea();
  char buff_location[70];
  char buff_prefix[30];
  char *data = "Kuala Lumpur, Malaysia|Partly cloudy@m";
  uint8_t len = strlen(data);
  uint8_t key_idx = 0;
  uint8_t prefix_idx = 0;
  for (uint8_t j=len-1;j>-1;j--) {
    if (data[j] == '@') {
      key_idx = j+1;
    } else if (data[j] == '|') {
      prefix_idx = j+1;
      break;
    }
  }
  uint8_t key_len = len - key_idx;
  uint8_t prefix_len = len - prefix_idx - key_len;
  snprintf(buff_location, len - prefix_len - key_len, "%s", data);
  snprintf(buff_prefix, prefix_len, "%s", (data + prefix_idx));
  char *key = (data + key_idx);
  uint8_t idx = findIndex(key);
  uint8_t y_axis = 12;
  LCD.setTextFont(1);
  for (uint8_t y=0;y<COLUMNS;y++) {
    LCD.drawString(weather_icons[idx][y], TFT_W / 4, y_axis);
    y_axis += 9;
  }
  y_axis -= 5;
  LCD.drawString(buff_location, floor((TFT_W - LCD.textWidth(buff_location)) / 2), y_axis);
  y_axis += 12;
  LCD.drawString(buff_prefix, floor((TFT_W - LCD.textWidth(buff_prefix)) / 2), y_axis);
}

static void _destroy() {}

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

Module weather = { "WEATHER", _init, _destroy, onKeyUp, onKeyDown,onKeyRight, onKeyLeft, onKeyMid, onKeySet, onKeyReset };

Module GetModuleWeather() {
  return weather;
}

#ifdef __cplusplus
}
#endif
