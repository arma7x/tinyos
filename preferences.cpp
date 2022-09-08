#include <Preferences.h>
#include "constant.h"

#ifdef __cplusplus
 extern "C" {
#endif

Preferences getPreferences() {
  Preferences preferences;
  preferences.begin("esp32", false);
  return preferences;
}

#ifdef __cplusplus
}
#endif
