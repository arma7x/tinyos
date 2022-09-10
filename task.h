#ifndef SYS_TASK_H
#define SYS_TASK_H


#ifdef __cplusplus
 extern "C" {
#endif

extern TaskHandle_t syncClockPid;

void TaskUpdateClock(void*);
void TaskSyncClock(void*);


#ifdef __cplusplus
}
#endif


#endif
