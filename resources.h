#ifndef RESOURCES_H
#define RESOURCES_H

#include <pgmspace.h>
#include <stdint.h>
#include "module.h"

#ifdef __cplusplus
 extern "C" {
#endif

extern const char keyboards[4][10] PROGMEM;
extern const uint8_t wallpaper[] PROGMEM;
extern const uint8_t epd_bitmap_weather[] PROGMEM;
extern const uint8_t epd_bitmap_foreign_currency[] PROGMEM;
extern const uint8_t epd_bitmap_thermometer[] PROGMEM;
extern const uint8_t epd_bitmap_calendar[] PROGMEM;
extern const uint8_t epd_bitmap_settings[] PROGMEM;
extern const uint8_t epd_bitmap_keyboard[] PROGMEM;
extern const uint8_t epd_bitmap_sun[] PROGMEM;
extern const uint8_t epd_bitmap_wi_fi[] PROGMEM;
extern const uint8_t epd_bitmap_clock[] PROGMEM;
extern const uint8_t epd_bitmap_thermometer2[] PROGMEM;
extern const uint8_t epd_bitmap_humidity[] PROGMEM;


#ifdef __cplusplus
}
#endif

#endif
