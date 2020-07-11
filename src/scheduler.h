
#ifndef SCHEDULER_H_
#define SCHEDULER_H_

/* Includes ------------------------------------------------------------------- */
#include "scheduler_cfg.h"
#include "system_types.h"





/***************************************************************************  
 	结构体定义 
***************************************************************************/

typedef struct
{
	UINT8 loop_context;				//内核工作模式
	UINT8 requested_context;		//需要切换的工作模式
	BOOL (*const *nvram_ptr)(void);
}Sche_Kernel_Struct;

//定时任务的结构体
typedef struct
{
	void  (*task_name)(void);
   	UINT8 reload_value;
   	UINT8 loop_membership;
}Sche_TimeTask_Struct;

//空闲任务的结构体
typedef struct 
{
	void (*task_name)(void);
    UBYTE loop_membership;
} Sche_Robin_Struct;

typedef struct
{
	UINT8 func_count[SCHE_NUM_FAST_TASKS + 1];
	BOOL func_flag[SCHE_NUM_FAST_TASKS + 1];
	const Sche_TimeTask_Struct func_task[SCHE_NUM_FAST_TASKS+ 1];
}Sche_FastTask_Struct;

typedef struct
{
	UINT8 func_posi;
	UINT8 func_count[SCHE_NUM_SLOW_TASKS + 1];
	BOOL func_flag[SCHE_NUM_SLOW_TASKS + 1];
	const Sche_TimeTask_Struct func_task[SCHE_NUM_SLOW_TASKS + 1];
}Sche_SlowTask_Struct;

typedef struct
{
	UINT8 func_posi;
	const Sche_Robin_Struct func_task[SCHE_NUM_ROBIN_TASK + 1];
}Sche_RobinTask_Struct;



UINT8 Sche_GetContext(void);
void Sche_SetContext(UINT8 requested_context);
void Sche_IsrBody(void);
void Sche_Service(void);

#endif /* SCHEDULER_H_ */

/**
 * @}
 */
    
/* --------------------------------- End Of File ------------------------------ */

