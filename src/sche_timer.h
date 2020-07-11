#ifndef _SCHE_TIMER_H_
#define _SCHE_TIMER_H_

void ScheTimer_Init(void);

void ScheTimer_disable_interrupt(void);
void ScheTimer_enable_interrupt(void);

UINT16 ScheTimer_GetSecCount(void);
UINT16 ScheTimer_Get4msCount(void);

#endif
