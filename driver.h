#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include <TFT_eSPI.h>
#include <SPI.h>

#ifdef __cplusplus
 extern "C" {
#endif

extern TFT_eSPI LCD;

void initLCD();
void clearSafeArea();
void setDisplayBrightness(uint8_t);
void sleepDisplay(bool);

#ifdef __cplusplus
}
#endif

#endif
