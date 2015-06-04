/******************************************************************************
 * FileName: app_main.c
 * Description: Alternate SDK (libmain.a)
 * Author: PV`
 * (c) PV` 2015
*******************************************************************************/
#include "user_config.h"

#ifdef USE_US_TIMER

#include "timer_us.h"
// us timer
// 0xFFFFFFFF/(80000000 >> 4) = 858.993459
// (80000000 >> 4)/(1000000>>2) = 20
// (80000000 >> 4)/1000000 = 5
// ms timer
// 0xFFFFFFFF/(80000000 >> 8) = 13743.895344
// (80000000 >> 8)/(1000>>2) = 1250
// (80000000 >> 8)/1000 = 312.5
#define XS_TO_RTC_TIMER_TICKS(t, prescaler, period)	\
     (((t) > (0xFFFFFFFF/(APB_CLK_FREQ >> prescaler))) ?	\
      (((t) >> 2) * ((APB_CLK_FREQ >> prescaler)/(period>>2)) + ((t) & 0x3) * ((APB_CLK_FREQ >> prescaler)/period))  :	\
      (((t) * (APB_CLK_FREQ >> prescaler)) / period))


void ICACHE_FLASH_ATTR ets_timer_arm_us(ETSTimer *ptimer, uint32_t us_ms, int repeat_flag)
{
	if(ptimer->timer_next != (ETSTimer *)0xffffffff) ets_timer_disarm(ptimer);
	if(us_ms != 0) {
#if ((APB_CLK_FREQ>>4)%1000000)
			 us_ms = XS_TO_RTC_TIMER_TICKS(us_ms, 4, 1000000);
#else
			us_ms *= (APB_CLK_FREQ>>4)/1000000;
#endif
	}
	if(repeat_flag) ptimer->timer_period = us_ms;
//	MEMW();
	timer_insert(TIMER1_COUNT + us_ms, ptimer);
}

/*
// ------------------------------------------------------
// system_timer_init_us
// устанавливает делитель таймера на :16 вместо :256
void ICACHE_FLASH_ATTR system_timer_init_us(void)
{
	TIMER1_CTRL = 0x84;
}
*/

#endif // USE_US_TIMER
