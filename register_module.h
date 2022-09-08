#ifndef REGISTER_MODULE_H
#define REGISTER_MODULE_H

#include "module.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct _menu {
  const unsigned char* icon;
  char* title;
} Menu;

Module GetModuleHomescreen();
Module GetModuleMenu();
Module GetModuleKeyboardUI();
Module GetModuleSettings();
Module GetModuleWiFi();


#ifdef __cplusplus
}
#endif

#endif
