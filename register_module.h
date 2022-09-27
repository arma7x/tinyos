#ifndef REGISTER_MODULE_H
#define REGISTER_MODULE_H

#include "types.h"

#ifdef __cplusplus
 extern "C" {
#endif

void ModuleSwitcher(Module);
Module GetActiveModule();

Module GetModuleHomescreen();
Module GetModuleMenu();
Module GetModuleKeyboardUI();
Module GetModuleSettings();
Module GetModuleWiFi();
Module GetModuleTemperature();
Module GetModuleWeather();
Module GetModuleSystemInfo();
Module GetModuleCurrency();


#ifdef __cplusplus
}
#endif

#endif
