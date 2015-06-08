/******************************************************************************
 * FileName: timer_us.h
 * Description: Alternate SDK (libmain.a)
 * Author: PV`
 * (c) PV` 2015
*******************************************************************************/

#ifndef _INCLUDE_TIMER_US_H_
#define _INCLUDE_TIMER_US_H_

#include "user_config.h"
#include "hw/esp8266.h"

void ets_timer_arm_us(ETSTimer *ptimer, uint32_t us_ms, int repeat_flag) ICACHE_FLASH_ATTR;

#define system_timer_init_us() TIMER1_CTRL = 0x84

#endif /* _INCLUDE_TIMER_US_H_ */
