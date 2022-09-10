#ifndef NOTIFICATION_BAR_H
#define NOTIFICATION_BAR_H


#ifdef __cplusplus
 extern "C" {
#endif

void drawClock(uint8_t, uint8_t, uint8_t);
void updateWifiStatus();
void setWifiStatus(uint8_t);
void InitNotificationBar();


#ifdef __cplusplus
}
#endif


#endif
