#ifndef ENV_H
#define ENV_H


#include <Preferences.h>

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


#ifdef __cplusplus
 extern "C" {
#endif

Preferences getPreferences();

#ifdef __cplusplus
}
#endif


#endif
