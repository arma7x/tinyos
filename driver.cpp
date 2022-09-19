#include "env.h"
#include "driver.h"
#include <Wire.h>
#include <WiFi.h>

#ifdef __cplusplus
 extern "C" {
#endif

void initDriver() {
  Wire.begin();
  WiFi.mode(WIFI_MODE_STA);
  WiFi.mode(WIFI_MODE_NULL);
  initDisplay();
  // initLightSensor();
}


#ifdef __cplusplus
}
#endif
