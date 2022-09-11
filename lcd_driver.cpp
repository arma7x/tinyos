#include <TFT_eSPI.h>
#include <SPI.h>
#include "env.h"

#ifdef __cplusplus
 extern "C" {
#endif


TFT_eSPI LCD = TFT_eSPI();

void setLcdBrightness(uint8_t v) {
  ledcWrite(BLK_C, v);
}

void initLCD() {
  LCD.init();
  LCD.setRotation(3);
  LCD.fillScreen(TFT_BG);

  uint8_t lcd_b = getPreferences().getUChar("lcd_b", 15);
  ledcSetup(BLK_C, 5000, 8);
  ledcAttachPin(BLK, BLK_C);
  setLcdBrightness(lcd_b);
}

void clearSafeArea() {
  LCD.fillRect(0, 10, 160, 70, TFT_BG);
}


#ifdef __cplusplus
}
#endif
