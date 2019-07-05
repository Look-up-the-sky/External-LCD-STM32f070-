#ifndef _LIB_TASK_H_
#define _LIB_TASK_H_

typedef void (*pFunc)(void);

void sInitTaskAdr(INT16U luwTaskNum, pFunc sFunction);
void sInitTaskPeriod(INT16U luwTaskNum, INT16U luwPeriod, INT16U luwStartTime);
void sTaskStart(void);
INT8U sTaskTimer(void);
void sSetTask(INT16U luwTaskNum);
void sSetTaskDelay(INT16U luwTaskNum,INT8U lubDelay);

#endif
