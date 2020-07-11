#define _SCHE_MCU_C_


#include "system_types.h"
#include "sche_mcu.h"



//检查是否有睡眠需求
BOOL ScheMcu_SleepRequest(void)
{
	return FALSE;
}

//使单片机进入睡眠状态
void ScheMcu_Sleep(void)
{
	//
}

void ScheMcu_UpdateWatchdog(void)
{
	//
}
			

void ScheMcu_DisableInterrupts(void)
{
	__disable_irq();
}

void ScheMcu_EnableInterrupts(void)
{
	__enable_irq();
}


void ScheMcu_ReportError(UINT8 error_code)
{
	ScheMcu_DisableInterrupts();
  
  	//NVIC_SystemReset();
  	while(1);
}




