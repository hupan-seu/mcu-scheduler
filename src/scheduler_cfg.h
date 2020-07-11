
#ifndef SCHEDULER_CFG_H_
#define SCHEDULER_CFG_H_


#include "gpio.h"
#include "dma.h"
#include "uart.h"

//#include "can_dll.h"
#include "led.h"
#include "Terminal.h"
#include "gprs.h"

//#include "idd01.h"
//#include "board.h"






//几种工作模式
#define  MODE_TEST   	0x08  		//测试模式
#define  MODE_DLD      	0x04  		//老化模式
#define  MODE_RE1       0x02  		//保留
#define  MODE_NORMAL    0x01  		//正常模式



//初始化内核模式
#define SCHE_INITIAL_CONTEXT	MODE_NORMAL


//冷启动函数列表
#define PROD_SPEC_COLD_START_LIST   \
			GPIO_ColdInit,\
			Dma_ColdInit,\
			LED_ColdInit,\
			TermUart_Init,\
			TermSend_Init,\
			TermRec_Init,\
			TermLog_Init,\
			Gprs_ColdInit,\
			/*CanDll_ColdInit,\
			CanIl_ColdInit,\
			DrivePar_ColdInit,\
			TBoxUpload_ColdInit,\
			TBoxCtrl_ColdInit,\
			TBoxUpdate_ColdInit,\
			TBoxSet_ColdInit,\
			*/



//RAM检查函数列表
#define PROD_SPEC_NVRAM_CHECK_LIST 	 \
		
		
//睡眠函数列表
#define PROD_SPEC_SLEEP_LIST 	\
			Gprs_Sleep,\
			TermLog_Sleep,\
			TermRec_Sleep,\
			TermSend_Sleep,\
			TermUart_Sleep,\
			LED_Sleep,\
			Dma_Sleep,\
			GPIO_Sleep,\
			/*TBoxSet_Sleep,\
			TBoxUpdate_Sleep,\
			TBoxCtrl_Sleep,\
			TBoxUpload_Sleep,\
			DrivePar_Sleep,\
			CanIl_Sleep,\
			CanDll_Sleep,\	
			EpmPkg_Sleep,\*/
			


//唤醒函数列表
#define PROD_SPEC_WAKEUP_LIST	\
			GPIO_Wakeup,\
			Dma_Wakeup,\
			LED_Wakeup,\
			TermUart_Wakeup,\
			TermSend_Wakeup,\
			TermRec_Wakeup,\
			TermLog_Wakeup,\
			Gprs_Wakeup,\
			/*CanDll_Wakeup,\
			CanIl_Wakeup,\
			DrivePar_Wakeup,\
			TBoxUpload_Wakeup,\
			TBoxCtrl_Wakeup,\
			TBoxUpdate_Wakeup,\
			TBoxSet_Wakeup,\
			*/

     
//RRobin函数列表和执行选项 
#define SCHE_NUM_ROBIN_TASK	2

#define  SCHE_LIST_ROBIN_TASK		\
	{LED_KSRRobin, MODE_NORMAL},\
	{TermLog_Task, MODE_NORMAL},\
	/*{CanIl_CheckTimeout_RRobin,	MODE_TEST|MODE_NORMAL},\*/
	
	

//快函数列表和执行选项 
#define SCHE_NUM_FAST_TASKS   2

#define SCHE_LIST_FAST_TASKS		\
	{Gprs_Task,  2, MODE_NORMAL},\
	{TermRec_Task, 2, MODE_NORMAL},\
	/*{CanDll_Task, 2, MODE_NORMAL},\*/
	/*{CanDll_Task,	1, MODE_TEST|MODE_NORMAL}, \*/


//慢函数列表和执行选项
#define SCHE_NUM_SLOW_TASKS    1

#define SCHE_LIST_SLOW_TASKS      \
	{TermSend_Task,  5, MODE_NORMAL},\
	/*{Board_Task, 50, MODE_NORMAL},\*/
	/*{TBoxUpload_Task, 20, MODE_TEST|MODE_NORMAL},	\*/
	/*{TBoxUpdate_Task, 40, MODE_TEST|MODE_NORMAL},	\	*/		



#endif /* SCHEDULER_CFG_H_ */

    
/* --------------------------------- End Of File ------------------------------ */


