#ifndef REGISTER_MODULE_H
#define REGISTER_MODULE_H

#include "module.h"

#ifdef __cplusplus
 extern "C" {
#endif

Module GetModuleHomescreen();
Module GetModuleMenu();
Module GetModuleKeyboardUI();


#ifdef __cplusplus
}
#endif

#endif
