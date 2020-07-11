
#define SW_TIMER_C_


  
#include "scheduler.h"
#include "sw_timer.h"

 


//每个软件定时器的 16 bit 定义如下:
//  
//  bit 15 (MSB) = TIMER_BASE_BIT			定时器最小单位，0代表4ms, 1代表512ms
//  bit 14       = TIMER_STATE_RUNNING		定时器运行状态
//  bits 0..13   = 14 bit value indicating the timeout time	定时器的超时值
//
#define TIMER_BASE_BIT                 ((UINT16) 0x8000u) /* 1=512ms; 0=4ms */
#define TIMER_STATE_RUNNING            ((UINT16) 0x4000u) /* 1=YES;  0=NO */



static UINT16 kernel_sw_timer_base_4ms;
static UINT16 kernel_sw_timer_base_512ms;
static UINT16 kernel_sw_timers[ MAX_SW_TIMER_ID ] ;



void Swtimer_UpdateCount(void)
{
	kernel_sw_timer_base_4ms = (kernel_sw_timer_base_4ms+1)  & 0x3FFF;		//更新软件计时器
    if ((kernel_sw_timer_base_4ms & 0x007F) == 0)
    {
    	kernel_sw_timer_base_512ms = (kernel_sw_timer_base_512ms+1)  & 0x3FFF;
    }

	return;
}

void SwTimer_Start(UINT16 timer_base, UINT16 timeout_time, SWTIMERS timer_id)
{

	UINT16 timer_value;

	if ((timeout_time & (UINT16)0x6000) != (UINT16)0)
	{
   		timeout_time = 0;
	}    


  	if ((timer_base & TIMER_13BIT_BASE_512MS) != (UINT16)0)
  	{
    	timer_value = kernel_sw_timer_base_512ms;
  	}
  	else
  	{
    	timer_value = kernel_sw_timer_base_4ms;
  	}

	kernel_sw_timers [ timer_id ]  = 
          ((((timeout_time & (UINT16)0x1FFF) + timer_value) & (UINT16)0x3FFF) 
          | TIMER_STATE_RUNNING) 
          | (timer_base & TIMER_BASE_BIT);

}

void SwTimer_Clear(SWTIMERS timer_id)
{
	kernel_sw_timers[timer_id] = 0;
}


UINT8 SwTimer_Check(SWTIMERS timer_id)
{
	UINT16 timer_ref;

	UINT16 *timer;

	timer = &kernel_sw_timers[ timer_id ];

	if ((*timer & TIMER_STATE_RUNNING) == (UINT16)0)
	{
    	return(TIMER_CLEAR);
	}

	/*  Timer is running...check to see if expiration time has been reached (
	**  obtain timer base reference before checking).*/
	if ((*timer & TIMER_13BIT_BASE_512MS) != (UINT16)0)
	{
    	timer_ref = kernel_sw_timer_base_512ms; 
	}    
	else
	{
    	timer_ref = kernel_sw_timer_base_4ms;
	}    

	if (((UINT16)(timer_ref - (*timer & (UINT16) 0x3FFF)) & (UINT16) 0x2000) != (UINT16)0)
	{
    	/*  Critical section is over.*/
    	return(TIMER_RUNNING);
	}    

	/*  Timer has expired, clear the TIMER_STATE_RUNNING bit.*/

    *timer = 0;   /* Existing line of code*/

	return(TIMER_EXPIRED);
}


