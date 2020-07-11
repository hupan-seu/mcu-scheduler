
#ifndef _SCHE_MCU_H_
#define _SCHE_MCU_H_


bool ScheMcu_SleepRequest(void);
void ScheMcu_Sleep(void);


void ScheMcu_UpdateWatchdog(void);


void ScheMcu_DisableInterrupts(void);
void ScheMcu_EnableInterrupts(void);


void ScheMcu_ReportError(UINT8 error_code);



#endif
