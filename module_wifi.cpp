#include <pgmspace.h>
#include <stdint.h>
#include "env.h"
#include "driver.h"
#include "module.h"
#include "register_module.h"
#include "resources.h"

#ifdef __cplusplus
 extern "C" {
#endif


static void _init(int num, ...) {
  clearSafeArea();
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

Module wifi = { _init, _destroy, onKeyUp, onKeyDown,onKeyRight, onKeyLeft, onKeyMid, onKeySet, onKeyReset };

Module GetModuleWiFi() {
  return wifi;
}

#ifdef __cplusplus
}
#endif
