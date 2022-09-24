#ifndef RESOURCES_H
#define RESOURCES_H

#include <pgmspace.h>
#include <stdint.h>

#ifdef __cplusplus
 extern "C" {
#endif

#define KEYBOARD_COLUMN 5
#define KEYBOARD_ROW 14

extern const char keyboards[KEYBOARD_COLUMN][KEYBOARD_ROW] PROGMEM;
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
extern const uint8_t epd_bitmap_globe[] PROGMEM;
extern const uint8_t epd_bitmap_menu_small[] PROGMEM;
extern const uint8_t epd_bitmap_gear_small[] PROGMEM;
extern const uint8_t epd_bitmap_wifi_small[] PROGMEM;


#ifdef __cplusplus
}
#endif

#endif
