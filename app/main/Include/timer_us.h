/*
 * timer_us.h
 *
 *  Created on: 13 мая 2015 г.
 *      Author: PVV
 */

#ifndef _INCLUDE_TIMER_US_H_
#define _INCLUDE_TIMER_US_H_

#include "user_config.h"
#include "bios/ets.h"
#include "hw/eagle_soc.h"
#include "hw/esp8266.h"

#ifdef USE_US_TIMER

void ets_timer_arm_us(ETSTimer *ptimer, uint32_t us_ms, int repeat_flag) ICACHE_FLASH_ATTR;

#define system_timer_init_us() TIMER1_CTRL = 0x84

#endif

#endif /* _INCLUDE_TIMER_US_H_ */
