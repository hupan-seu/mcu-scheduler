#ifndef SW_TIMER_H_
#define SW_TIMER_H_

typedef enum
{
		SW_TIMERS_TERM_LOG_PERIOD,
		SW_TIMERS_LED1_BLINK_TIMEOUT,
		SW_TIMERS_LED2_BLINK_TIMEOUT,
		SW_TIMERS_GPRS_STATE_TIMEOUT,
		SW_TIMERS_CAN_SEND_TIMEOUT,
		SW_TIMERS_GPRS_SEND_TIMEOUT,
		SW_TIMERS_UART1_RE_DELAY,
		SW_TIMERS_UART1_SEND_DELAY,
		SW_TIMERS_BOARD_45S_CHECK_TIMEOUT,
		SW_TIMERS_BOARD_2S_CHECK_TIMEOUT,
		SW_TIMERS_BOARD_23VUP_TIMEOUT,
		SW_TIMERS_BOARD_0LOG_TIMEOUT,
		SW_TIMERS_BOARD_1LOG_TIMEOUT,
		SW_TIMERS_BOARD_2LOG_TIMEOUT,
		SW_TIMERS_BOARD_3LOG_TIMEOUT,
		SW_TIMERS_BOARD_STATE_TIMEOUT,
		SW_TIMERS_BOARD_POWER_TIMEOUT,
		SW_TIMERS_BOARD_TERM_RCV_TIMEOUT,
		SW_TIMERS_BOARD_CAN_CHECK_TIMEOUT,
		SW_TIMERS_BOARD_SELF_TEST_TIMEOUT,
        PROD_SPEC_SW_TIMERS_RCV_TIMEOUT,
		PROD_SPEC_SW_TIMERS_GPRS_DEL_ALL_MSG,
		PROD_SPEC_SW_TIMERS_GPRS_STATE_TIMEOUT,
		PROD_SPEC_SW_TIMERS_GPRS_LOCK_TIMEOUT,
		PROD_SPEC_SW_TIMERS_GPRS_WINDOW_TIMEOUT,
		PROD_SPEC_SW_TIMERS_GPRS_ENGINE_TIMEOUT,
        PROD_SPEC_SW_TIMERS_CAN_SEND_CMD_DELAY,
        PROD_SPEC_SW_TIMERS_CAN_WAKEUP_LSCMD_DELAY,
   		//
   		MAX_SW_TIMER_ID
} SWTIMERS;

/* Values returned from KernelCheck13BitTimer().*/
#define   TIMER_CLEAR       0x00
#define   TIMER_EXPIRED     0x02
#define   TIMER_RUNNING     0x80

/*Valid values for timer base for the KernelStart13BitTimer() service.*/
#define TIMER_13BIT_BASE_4MS    ((UINT16)0x0000u)
#define TIMER_13BIT_BASE_512MS  ((UINT16)0x8000u)

void Swtimer_UpdateCount(void);
void SwTimer_Start(UINT16 timer_base, UINT16 timeout_time, SWTIMERS timer_id);
void SwTimer_Clear(SWTIMERS timer_id);
UINT8 SwTimer_Check(SWTIMERS timer_id);

#endif
