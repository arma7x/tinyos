#ifndef SYS_TASK_H
#define SYS_TASK_H


#ifdef __cplusplus
 extern "C" {
#endif

extern TaskHandle_t SyncClockPid;
extern TaskHandle_t WatchWifiConnectionPid;

void TaskUpdateClock(void*);
void TaskSyncClock(void*);
void TaskWatchWifiConnection(void*);
void TaskGC(void*);


#ifdef __cplusplus
}
#endif


#endif
