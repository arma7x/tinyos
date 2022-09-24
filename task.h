#ifndef SYS_TASK_H
#define SYS_TASK_H


#ifdef __cplusplus
 extern "C" {
#endif

extern TaskHandle_t SyncClockPid;
extern TaskHandle_t WatchWifiConnectionPid;
extern TaskHandle_t ConnectToWifiPid;
extern time_t ConnectToWifiTime;

int calcTimezoneOffset();
void TaskUpdateClock(void*);
void TaskSyncClock(void*);
void TaskWatchWifiConnection(void*);
void TaskConnectToWifi(void*);
void TaskGC(void*);


#ifdef __cplusplus
}
#endif


#endif
