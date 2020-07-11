/******************************************************************************
  
         CONFIDENTIAL - Auto_Linked



******************************************************************************/
#define _SCHE_TIMER_C_


#include "system_types.h"
#include "stm32f0xx_misc.h"
#include "stm32f0xx_rcc.h"		//各个模块时钟
#include "stm32f0xx_tim.h"

#include "scheduler.h"
#include "sche_timer.h"



static volatile UINT16 CountPer4msTo100;
static volatile UINT16 Count_Sec;						//提供比较精确的秒计时, 外部文件禁止修改
static volatile UINT16 CountPer4ms;


static void ScheTimer_CountTick(void);

/*void ScheTimer_Init(void)
{

    TIM_TIMERCFG_Type TIM_ConfigStruct;
    TIM_MATCHCFG_Type TIM_MatchConfigStruct ;
  
   
    TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;				// Initialize timer 0, prescale count time of 10uS
    TIM_ConfigStruct.PrescaleValue  = 10;

	TIM_Init(LPC_TIM0, TIM_TIMER_MODE,&TIM_ConfigStruct);				// Set configuration for Tim_config and Tim_MatchConfig
    
	//捕获通道0, 内核调度使用
	TIM_MatchConfigStruct.MatchChannel = 0;								// use channel 0, MR0
    TIM_MatchConfigStruct.IntOnMatch   = TRUE;							// Enable interrupt when MR0 matches the value in TC register
    TIM_MatchConfigStruct.ResetOnMatch = FALSE;
    TIM_MatchConfigStruct.StopOnMatch  = FALSE;							//Stop on MR0 if MR0 matches it
    TIM_MatchConfigStruct.ExtMatchOutputType =TIM_EXTMATCH_NOTHING;		//Toggle MR0.0 pin if MR0 matches it
    TIM_MatchConfigStruct.MatchValue   = LPC_TIM0->TC + 400;			// Set Match value, count value of 400 (400 * 10uS =4mS --> 250 Hz)
    TIM_ConfigMatch(LPC_TIM0, &TIM_MatchConfigStruct);*/
   
    //NVIC_SetPriority(TIMER0_IRQn, ((0x01<<3)|0x00));					/* preemption = 1, sub-priority = 1 */
   
    //NVIC_EnableIRQ(TIMER0_IRQn);										/* Enable interrupt for timer 0 */
    
    //TIM_Cmd(LPC_TIM0,ENABLE);											// To start timer 0

	
	//Count_Sec = 0;
	//CountPer4msTo100 = 0;
	//CountPer4ms = 0;

	//return;
//}

void ScheTimer_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;				//设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStructure);
								 
	//打开定时器2的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	//基本配置
	TIM_TimeBaseStructure.TIM_Period = 65535;					//计数溢出大小
	TIM_TimeBaseStructure.TIM_Prescaler = 4800-1;				//预分频系数  48MHz/4800=100us
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//时钟分割?
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	

	//捕获通道1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 40;							//100us*40=4ms
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Disable);
	
	//
	TIM_ITConfig(TIM3,TIM_IT_CC1, ENABLE);
	
	//
	TIM_Cmd(TIM3,ENABLE);
	
	Count_Sec = 0;
	CountPer4msTo100 = 0;
	CountPer4ms = 0;
}

//禁止内核调度中断
void ScheTimer_disable_interrupt(void)
{
	TIM_ITConfig(TIM3,TIM_IT_CC1, DISABLE);
}

//使能内核调度中断
void ScheTimer_enable_interrupt(void)
{
	TIM_ITConfig(TIM3,TIM_IT_CC1, ENABLE);  
}


/*void TIMER0_IRQHandler(void)
{

	if (TIM_GetIntStatus(LPC_TIM0,0))
	{
		LPC_TIM0->MR0 = LPC_TIM0->TC + 400;
  		TIM_ClearIntPending(LPC_TIM0, TIM_MR0_INT);

		Sche_IsrBody();
		ScheTimer_CountTick();
	}
	else
	{}



	return;
}*/

void TIM3_IRQHandler(void)
{
	uint16_t capture = 0;

	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		capture = TIM_GetCapture1(TIM3);
		TIM_SetCompare1(TIM3, capture + 40); 	//???4ms???

		//????
		Sche_IsrBody();
		ScheTimer_CountTick();
	}

	return;
}

UINT16 ScheTimer_GetSecCount(void)
{
	return Count_Sec;
}

UINT16 ScheTimer_Get4msCount(void)
{
	return CountPer4ms;
}


static void ScheTimer_CountTick(void)
{      
	CountPer4ms++;
	CountPer4msTo100++;

	if(CountPer4msTo100 >= 250)
	{
		CountPer4msTo100 = 0;
		Count_Sec++;
	}

	return;
}




