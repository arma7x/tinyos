#ifndef ENV_H
#define ENV_H


#include <Preferences.h>

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

#define BLK_C 0
#define BLK 16
#define RST 33
#define SET 25
#define MID 26
#define RHT 27
#define LFT 14
#define DWN 12
#define UP  13

#define TFT_GREY 0x7BEF
#define TFT_BG  0xCE59
#define TFT_W 160
#define TFT_H 80

#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 28800
#define DAYLIGHT_OFFSET_SEC 0

#define ON "ON"
#define OFF "OFF"

#define TZ_NUM 41

#ifdef __cplusplus
 extern "C" {
#endif

extern const char *TZ_LIST[TZ_NUM];

Preferences getPreferences();

#ifdef __cplusplus
}
#endif


#endif
