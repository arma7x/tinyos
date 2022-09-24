#ifndef NOTIFICATION_BAR_H
#define NOTIFICATION_BAR_H


#ifdef __cplusplus
extern "C" {
#endif

  void drawClock(uint8_t, uint8_t, uint8_t);
  void updateWifiStatus();
  void setWifiStatus(bool);
  void initNotificationBar();


#ifdef __cplusplus
}
#endif


#endif