#include <TFT_eSPI.h>
#include <SPI.h>
#include "constant.h"

#ifdef __cplusplus
 extern "C" {
#endif


TFT_eSPI LCD = TFT_eSPI();

void initLCD() {
  LCD.init();
  LCD.setRotation(3);
  LCD.fillScreen(TFT_BG);
}

void clearSafeArea() {
  LCD.fillRect(0, 10, 160, 70, TFT_BG);
}


#ifdef __cplusplus
}
#endif
