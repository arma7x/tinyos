#include <WiFi.h>
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


static void _init(int num, ...) {
  clearDisplaySafeArea();
  uint8_t pad_top = 10;
  LCD.setFreeFont(&FreeSans9pt7b);
  LCD.setTextFont(1);
  byte mac[6];
  esp_chip_info_t chip_info;
  uint32_t flash_size;
  WiFi.macAddress(mac);
  esp_chip_info(&chip_info);
  char tpl[100];
  
  sprintf(tpl, "%s %d core WiFi%s%s", CONFIG_IDF_TARGET, chip_info.cores, (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "", (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");
  LCD.drawString(tpl, 1, pad_top);
  pad_top += 9;
  memset(&tpl[0], 0, sizeof(tpl));
  
  sprintf(tpl, "Heap: %.0fKB, Psram: %.0fKB", floor(ESP.getHeapSize() / 1000), floor(ESP.getPsramSize() / 1000));
  LCD.drawString(tpl, 1, pad_top);
  pad_top += 9;
  memset(&tpl[0], 0, sizeof(tpl));
  
  sprintf(tpl, "Chip: %s, rev%d", ESP.getChipModel(), ESP.getChipRevision());
  LCD.drawString(tpl, 1, pad_top);
  pad_top += 9;
  memset(&tpl[0], 0, sizeof(tpl));
  
  sprintf(tpl, "Cpu Freq: %dMhz", ESP.getCpuFreqMHz());
  LCD.drawString(tpl, 1, pad_top);
  pad_top += 9;
  memset(&tpl[0], 0, sizeof(tpl));
  
  sprintf(tpl, "SDK: %s", ESP.getSdkVersion());
  LCD.drawString(tpl, 1, pad_top);
  pad_top += 9;
  memset(&tpl[0], 0, sizeof(tpl));
  
  sprintf(tpl, "Flash Chip Size: %.0fMB", floor(ESP.getFlashChipSize() / 1000000));
  LCD.drawString(tpl, 1, pad_top);
  pad_top += 9;
  memset(&tpl[0], 0, sizeof(tpl));
  
  // sprintf(tpl, "Flash Chip Speed: %dMhz", ESP.getFlashChipSpeed() / 1000000);
  // LCD.drawString(tpl, 1, pad_top);
  // pad_top += 9;
  // memset(&tpl[0], 0, sizeof(tpl));
  
  // sprintf(tpl, "Sketch Space: %d bytes", ESP.getFreeSketchSpace());
  // LCD.drawString(tpl, 1, pad_top);
  // pad_top += 9;
  // memset(&tpl[0], 0, sizeof(tpl));
  
  // sprintf(tpl, "Sketch Size: %d bytes", ESP.getSketchSize());
  // LCD.drawString(tpl, 1, pad_top);
  // pad_top += 9;
  // memset(&tpl[0], 0, sizeof(tpl));
  
  sprintf(tpl, "Free Sketch Space: %.0fKB", floor((ESP.getFreeSketchSpace() - ESP.getSketchSize()) / 1000));
  LCD.drawString(tpl, 1, pad_top);
  pad_top += 9;
  memset(&tpl[0], 0, sizeof(tpl));
  
  sprintf(tpl, "MAC: %02X:%02X:%02X:%02X:%02X:%02X", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  LCD.drawString(tpl, 1, pad_top);
  pad_top += 9;
  memset(&tpl[0], 0, sizeof(tpl));
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

Module system_info = { "SYSTEM_INFO", _init, _destroy, onKeyUp, onKeyDown,onKeyRight, onKeyLeft, onKeyMid, onKeySet, onKeyReset };

Module GetModuleSystemInfo() {
  return system_info;
}

#ifdef __cplusplus
}
#endif
