#ifndef NOTIFICATION_BAR_H
#define NOTIFICATION_BAR_H


#ifdef __cplusplus
extern "C" {
#endif

void drawClockWidget(struct tm*);
void drawHomescreenWidget(struct tm*);
void updateWifiStatus();
void setWifiStatus(bool);
void initNotificationBar();


#ifdef __cplusplus
}
#endif


#endif
