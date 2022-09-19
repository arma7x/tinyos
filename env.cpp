#include <Preferences.h>
#include "env.h"

#ifdef __cplusplus
 extern "C" {
#endif

Preferences getPreferences() {
  Preferences preferences;
  preferences.begin("esp32", false);
  return preferences;
}

const char *TZ_LIST[TZ_NUM] = { "+01", "+02", "+03", "+03:30", "+04", "+04:30", "+05", "+05:30",
"+05:45", "+06", "+06:30", "+07", "+08", "+08:45", "+09", "+09:30", "+10", "+10:30",
"+11", "+11:00", "+12", "+12:45", "+13", "+13:45", "+14", "-01", "-02", "-02:30",
"-03", "-03:30", "-04", "-04:30", "-05", "-06", "-07", "-08", "-09", "-09:30",
"-10", "-11", "-12" };

#ifdef __cplusplus
}
#endif
