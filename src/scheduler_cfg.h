
#ifndef SCHEDULER_CFG_H_
#define SCHEDULER_CFG_H_

// 添加自己任务的头文件
// #include "task1.h"
// #include "task2.h"
// #include "task3.h"
// #include "task4.h"
// #include "task5.h"

/**
 * 自定义几种内核工作模式，每个任务可以指定只在特定的模式下才会运行
 * 例如正常工作模式，产线测试模式，产线老化模式等
 * 每种模式占1bit
*/
#define  SCHE_MODE_1    0x01  		
#define  SCHE_MODE_2    0x02  		
#define  SCHE_MODE_3    0x04  		
#define  SCHE_MODE_4   	0x08  	

// 初始化内核模式
#define SCHE_INITIAL_CONTEXT	SCHE_MODE_1

// 任务初始化函数列表
#define PROD_SPEC_COLD_START_LIST \
			Task1_ColdInit,\
			Task2_ColdInit,\
			Task3_ColdInit,\
			Task4_ColdInit,\
			Task5_ColdInit,\

// 任务检查检查函数列表
#define PROD_SPEC_NVRAM_CHECK_LIST \
			Task1_Check,\
			
// 单片机休眠前，任务函数列表
#define PROD_SPEC_SLEEP_LIST \
			Task1_Sleep,\
			Task2_Sleep,\
			Task3_Sleep,\
			Task4_Sleep,\
			Task5_Sleep,\
			
// 单片机从休眠唤醒后，任务函数列表
#define PROD_SPEC_WAKEUP_LIST \
			Task1_Wakeup,\
			Task2_Wakeup,\
			Task3_Wakeup,\
			Task4_Wakeup,\
			Task5_Wakeup,\
     
// 快定时任务，运行函数列表 
#define SCHE_NUM_FAST_TASKS 2
#define SCHE_LIST_FAST_TASKS		\
	{Task1_Run, 2, SCHE_MODE_1|SCHE_MODE_2},\
	{Task2_Run, 1, SCHE_MODE_3},\


// 慢定时任务，运行函数列表 
#define SCHE_NUM_SLOW_TASKS 1
#define SCHE_LIST_SLOW_TASKS \
	{Task3_Run,  5, SCHE_MODE_1},\		

// 空闲任务，运行函数列表 
#define SCHE_NUM_ROBIN_TASK	2
#define  SCHE_LIST_ROBIN_TASK \
	{Task4_Run, SCHE_MODE_1},\
	{Task5_Run, SCHE_MODE_1},\

#endif
