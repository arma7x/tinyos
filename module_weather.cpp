#include <WiFi.h>
#include <WiFiClientSecure.h>
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

const char* le_root_ca PROGMEM = \
  "-----BEGIN CERTIFICATE-----\n" \
  "MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/\n" \
  "MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
  "DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow\n" \
  "PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD\n" \
  "Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n" \
  "AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O\n" \
  "rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq\n" \
  "OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b\n" \
  "xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw\n" \
  "7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD\n" \
  "aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\n" \
  "HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG\n" \
  "SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69\n" \
  "ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr\n" \
  "AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz\n" \
  "R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5\n" \
  "JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo\n" \
  "Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ\n" \
  "-----END CERTIFICATE-----\n";
     
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
  WiFiClientSecure client;
  if (WiFi.status() == WL_CONNECTED) {
    LCD.setFreeFont(&FreeSans9pt7b);
    LCD.setTextFont(1);
    LCD.drawString(F("Connecting..."), floor((TFT_W - LCD.textWidth(F("Connecting..."))) / 2), 35);
    client.setCACert(le_root_ca);
    if (!client.connect("wttr.in", 443)) {
      LCD.setFreeFont(&FreeSans9pt7b);
      LCD.setTextFont(1);
      LCD.drawString(F("Connection failed!"), floor((TFT_W - LCD.textWidth(F("Connection failed!"))) / 2), 35);
    } else {
      clearDisplaySafeArea();
      client.println("GET https://wttr.in/?format=%l|%C@%x HTTP/1.0");
      client.println("Host: wttr.in");
      client.println("Connection: close");
      client.println();

      while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          break;
        }
      }
      uint8_t len = 1;
      char *data = (char*) malloc(sizeof(char) * len);
      while (client.available()) {
        char c = client.read();
        if (c <= 127) { 
          data[len - 1] = c;
          data = (char*) realloc(data, sizeof(char) * ++len);
          // Serial.printf("%d %c %d\n", len - 2, data[len - 2], len - 1);
        }
      }
      data[len - 1] = '\0';
      // Serial.println(data);
      char buff_location[70];
      char buff_prefix[30];
      // char *data = "Kuala Lumpur, Malaysia|Partly cloudy@m";
      // uint8_t len = strlen(data);
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
      free(data);
      client.stop();
    }
  } else {
    LCD.setFreeFont(&FreeSans9pt7b);
    LCD.setTextFont(1);
    LCD.drawString(F("No Connection"), floor((TFT_W - LCD.textWidth(F("No Connection"))) / 2), 35);
  }
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
