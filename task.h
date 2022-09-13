#ifndef SYS_TASK_H
#define SYS_TASK_H


#ifdef __cplusplus
 extern "C" {
#endif

extern TaskHandle_t syncClockPid;
extern TaskHandle_t watchWifiConnectionPid;

void TaskUpdateClock(void*);
void TaskSyncClock(void*);
void TaskWatchWifiConnection(void*);


#ifdef __cplusplus
}
#endif


#endif
