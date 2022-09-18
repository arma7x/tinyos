#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include <TFT_eSPI.h>
#include <SPI.h>
#include <BH1750.h>

#ifdef __cplusplus
 extern "C" {
#endif

extern TFT_eSPI LCD;
extern BH1750 lightMeter;

void initDriver();
void initDisplay();
void clearDisplaySafeArea();
void setDisplayBrightness(uint8_t);
void sleepDisplay(bool);
void initLightSensor();

#ifdef __cplusplus
}
#endif

#endif
