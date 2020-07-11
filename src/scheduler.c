#define SCHED_C

#include <stdio.h>
#include "sche_types.h"

#include "scheduler_cfg.h"

#include "sche_timer.h"
#include "sche_mcu.h"
#include "scheduler.h"
#include "sw_timer.h"

/**
 * 静态函数声明 
*/
static void Sche_TimedCounters(void);						// 定时任务计数并更新标志位
static void Sche_Init(void);								// 内核初始化
static BOOL Sche_RunFastTask(void);							// 检查快任务标志位并执行任务
static BOOL Sche_RunSlowTask(void);							// 慢函数每次只执行一个
static BOOL Sche_RunRobinTask(void);						// 执行了一整轮之后返回true
static void Sche_RunVoidLst(void (*const (*top))(void));	// 执行 睡眠 苏醒 RAM检查等列表函数
static void Sche_InitVRam(void);							// 检查任务数量定义是否和实际一致
static void Sche_NvRamCheckSignal(void);
static BOOL Sche_NvRamCheckAll(void);
static void Sche_UpdateContext(void);						// 更新内核状态
static void Sche_RunScheduler(void);						// 循环执行内核任务，直到有单片机休眠需求时退出
	
/**
 * 变量定义
*/
static Sche_Kernel_Struct scheKernel;

static Sche_FastTask_Struct fastTask = {
		{0},
		{0},
		{
			SCHE_LIST_FAST_TASKS
			{NULL, 0, 0},
		},
};

static Sche_SlowTask_Struct slowTask = {
		{0},
		{0},
		{0},
		{
			SCHE_LIST_SLOW_TASKS
			{NULL, 0, 0},
		},
};

static Sche_RobinTask_Struct robinTask = 
{
		{0},
		{
			SCHE_LIST_ROBIN_TASK
			{NULL, 0},
		},
};


/**
 * 冷启动函数列表
*/
static void (*const scheColdInitListRom[])(void) = 
{
    PROD_SPEC_COLD_START_LIST
    (void(*)(void))0
};

/**
 * RAM检查函数列表
*/
static BOOL (*const scheRamCheckListRom[])(void)  = 
{
    PROD_SPEC_NVRAM_CHECK_LIST
    (BOOL (*)(void)) 0
};

/**
 * 睡眠函数列表
*/
static void (*const scheSleepListRom[])(void)  = 
{
	PROD_SPEC_SLEEP_LIST
    (void (*)(void)) 0
};

/**
 * 唤醒函数列表
*/
static void (*const scheWakeupListRom[])(void)  = 
{
    PROD_SPEC_WAKEUP_LIST   
    (void (*)(void)) 0
};
 
/**
 * 定时任务计数并更新标志位
*/
static void Sche_TimedCounters(void)
{
	UINT8 i;

	for(i=0; i<SCHE_NUM_FAST_TASKS; i++)
	{
		if(fastTask.func_flag[i] == TRUE)
		{
			continue;
		}
		
		if(fastTask.func_count[i] > 0)			//不正常状态
		{
			fastTask.func_count[i]--;
		}
		if(fastTask.func_count[i] == 0)
		{
			fastTask.func_count[i] = fastTask.func_task[i].reload_value;
			fastTask.func_flag[i] = TRUE;
		}
	}

	//
	for(i=0; i<SCHE_NUM_SLOW_TASKS; i++)
	{
		if(slowTask.func_flag[i] == TRUE)
		{
			continue;
		}
			
		if(slowTask.func_count[i] > 0)
		{
			slowTask.func_count[i]--;
		}
		if(slowTask.func_count[i] == 0)
		{
			slowTask.func_count[i] = slowTask.func_task[i].reload_value;
			slowTask.func_flag[i] = TRUE;
		}
	}
	
	return;
}


/**
 * something
*/
static void Sche_Init(void)
{
	UINT8 i;

	// 初始化内核状态
	scheKernel.loop_context = SCHE_INITIAL_CONTEXT; 
	scheKernel.requested_context = SCHE_INITIAL_CONTEXT; 
	scheKernel.nvram_ptr = scheRamCheckListRom;

	// 初始化Robbin任务指针
    robinTask.func_posi = 0;

	// 初始化快任务
    for(i=0; i<SCHE_NUM_FAST_TASKS; i++)
    {
    	fastTask.func_count[i] = fastTask.func_task[i].reload_value;
		fastTask.func_flag[i] = FALSE;
    }

	// 初始化慢任务
	slowTask.func_posi = 0;
	for(i=0; i<SCHE_NUM_FAST_TASKS; i++)
	{
		slowTask.func_count[i] = slowTask.func_task[i].reload_value;
		slowTask.func_flag[i] = FALSE;
	}

	return;
}

/**
 * 检查快任务标志位，执行任务
*/
static BOOL Sche_RunFastTask(void)
{
	UINT8 i;
	BOOL re_val;

	re_val = FALSE;

	for(i=0; i<SCHE_NUM_FAST_TASKS; i++)
	{
		if(fastTask.func_flag[i] == TRUE)
		{
			fastTask.func_flag[i] = FALSE;
			//
			if((fastTask.func_task[i].loop_membership & scheKernel.loop_context) != 0)
			{
				fastTask.func_task[i].task_name();
				re_val = TRUE;
			}
		}
	}
	
	return re_val;
}

/**
 * 检查慢任务标志位，执行任务
 * 慢任务每次只执行一个
*/
static BOOL Sche_RunSlowTask(void)
{
	UINT8 i,index;
	BOOL run_flag;

	i = 0;
	run_flag = FALSE;
	
	while(i < SCHE_NUM_SLOW_TASKS)
	{
		index = slowTask.func_posi;
		if(slowTask.func_flag[index] == TRUE)
		{
			slowTask.func_flag[index] = FALSE;
			if((slowTask.func_task[index].loop_membership & scheKernel.loop_context) != 0)
			{
				slowTask.func_task[index].task_name();
				run_flag = TRUE;
			}
		}

		i++;
		slowTask.func_posi++;
		if(slowTask.func_posi >= SCHE_NUM_SLOW_TASKS)
		{
			slowTask.func_posi = 0;
		}

		if(run_flag == TRUE)
		{
			break;
		}
	}

	return run_flag;
}

/**
 * 执行空闲任务
*/
static BOOL Sche_RunRobinTask(void)
{
	UINT8 index;
	
	if(robinTask.func_posi >= SCHE_NUM_ROBIN_TASK)
	{
		robinTask.func_posi = 0;
		return TRUE;
	}

	index = robinTask.func_posi;
	if((robinTask.func_task[index].loop_membership & scheKernel.loop_context) != 0)
	{
		robinTask.func_task[index].task_name();
	}

	robinTask.func_posi++;
	if(robinTask.func_posi >= SCHE_NUM_ROBIN_TASK)
	{
		robinTask.func_posi = 0;
		return TRUE;
	}

	return FALSE;
}

/**
 * 执行睡眠、苏醒、RAM检查等列表函数
*/
static void Sche_RunVoidLst(void (*const (*top))(void))
{
    if (top != ((void *) 0))
    {
        while(*top != ((void (*)(void)) 0) )  	/* While not NULL entry.*/
        {
             (**top)();      						/* Call function.*/
             top++;          						/* Advance pointer.*/
        }
    }

	return;
}

/**
 * 检查任务数量定义是否和实际一致
*/
static void Sche_InitVRam(void)
{
   
}

/**
 * something
*/
static void Sche_NvRamCheckSignal(void)
{
	if (*scheKernel.nvram_ptr == NULL)
    {
    	scheKernel.nvram_ptr = scheRamCheckListRom;  
    }
    else
    {
    	if (((*scheKernel.nvram_ptr)()) != TRUE)
        {
        	ScheMcu_ReportError(0x08);
        }
        ++scheKernel.nvram_ptr;
    }

	return;
}

static BOOL Sche_NvRamCheckAll(void)
{
	BOOL (*const *nvram_list_ptr)(void);

	nvram_list_ptr = scheRamCheckListRom;
	while (*nvram_list_ptr != ((BOOL (*)(void)) 0))
	{
	     if (((*nvram_list_ptr)()) == FALSE)
	     {
	          return FALSE;
	     }
	     ++nvram_list_ptr;
	}

	return TRUE;
}

/**
 * something
*/
static void Sche_UpdateContext(void)
{
}

/**
 * 循环执行内核任务，直到有单片机休眠需求时退出
*/
static void Sche_RunScheduler(void)
{
	BOOL func_flag;

	while(1)
	{      
		func_flag = Sche_RunFastTask();				// 快任务的优先级最高
		if(func_flag == TRUE)
		{
			continue;
		}

		func_flag = Sche_RunSlowTask();				// 慢函数每次只执行一个
		if(func_flag == TRUE)
		{
			continue;
		}
							
		func_flag = Sche_RunRobinTask();			// 执行 RRobin 函数列表,每次循环只执行一个函数
		if(func_flag == FALSE)						// 只有在空闲任务执行了一轮之后，才做下面的工作 
		{
			continue;
		}
		
		if (ScheMcu_SleepRequest() == TRUE) 		// 检查睡眠需求
		{
			break; 
		}			
		
        Sche_UpdateContext();						// 更新内核状态  
        ScheMcu_UpdateWatchdog();   				// 更新看门狗
		Sche_NvRamCheckSignal();					// 执行内存检查
	}

	return;
}

UINT8 Sche_GetContext(void)
{
    return(scheKernel.loop_context);
}

void Sche_SetContext(UINT8 requested_context)
{
	scheKernel.requested_context = requested_context;

    return;
}  
   
/**
 * something
*/
void Sche_IsrBody(void)
{
	Swtimer_UpdateCount();				// 更新软件定时器
   	Sche_TimedCounters(); 				// 更新定时任务标记   
}

/**
 * something
*/
void Sche_Service(void)
{
	ScheMcu_DisableInterrupts();
	Sche_InitVRam();								// 检查任务宏定义和任务数量是否一致
    Sche_RunVoidLst(scheColdInitListRom);  			// 执行冷启动函数列表
	ScheMcu_EnableInterrupts();

	while(1)
	{
		Sche_Init();
		ScheTimer_enable_interrupt();
		Sche_RunScheduler();
		ScheTimer_disable_interrupt();

		Sche_RunVoidLst(scheSleepListRom);			// 执行睡眠函数列表
		ScheMcu_Sleep(); 							// 使单片机进入睡眠状态

		ScheMcu_DisableInterrupts();
		if (Sche_NvRamCheckAll() == FALSE) 			// 执行NvRamCheck列表函数
		{ 
			ScheMcu_ReportError(0x0A); 
		}	
		Sche_InitVRam();							// 检查任务宏定义和任务数量是否一致
		Sche_RunVoidLst(scheWakeupListRom);  		// 执行wakeup列表函数
		ScheMcu_EnableInterrupts();
	}

}
