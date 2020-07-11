#define _SCHE_MCU_C_

#include "sche_types.h"
#include "sche_mcu.h"

/**
 * 检查是否有睡眠需求
 * 在这里添加自定义的检查逻辑
*/
BOOL ScheMcu_SleepRequest(void)
{
	return FALSE;
}

/**
 * 使单片机进入睡眠状态
 * 在这里添加对单片机硬件的操作指令
*/
void ScheMcu_Sleep(void)
{
	//
}

/**
 * 更新看门狗
 * 在这里添加对单片机硬件的操作指令
*/
void ScheMcu_UpdateWatchdog(void)
{
	//
}
			
/**
 * 关中断
*/
void ScheMcu_DisableInterrupts(void)
{
	__disable_irq();
}

/**
 * 开中断
*/
void ScheMcu_EnableInterrupts(void)
{
	__enable_irq();
}

/**
 * 运行错误
*/
void ScheMcu_ReportError(UINT8 error_code)
{
	ScheMcu_DisableInterrupts();
  
  	//NVIC_SystemReset();
  	while(1);
}
