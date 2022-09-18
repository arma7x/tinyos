#include <TFT_eSPI.h>
#include <SPI.h>
#include "env.h"

#include <Wire.h>
#include <BH1750.h>

#ifdef __cplusplus
 extern "C" {
#endif


BH1750 lightMeter;

void initLightSensor() {
  Wire.begin();
  lightMeter.begin();
}


#ifdef __cplusplus
}
#endif
