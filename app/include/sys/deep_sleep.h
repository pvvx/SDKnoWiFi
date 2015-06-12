/******************************************************************************
 * FileName: deep_sleep.h
 * Description: Alternate SDK (libmain.a)
 * Author: PV`
 * (c) PV` 2015
*******************************************************************************/

#ifndef _INCLUDE_DEEP_SLEEP_H_
#define _INCLUDE_DEEP_SLEEP_H_

void go_deep_sleep(uint32 x) ICACHE_FLASH_ATTR;
uint32 rtc_calib(uint32 k_xtal_mhz) ICACHE_FLASH_ATTR;
#define rtc_calibrate(fmhz) rtc_calib(4096/fmhz)
/* полный sleep, с перезагрузкой
 * минимум time_us = 8192 */
void sys_deep_sleep(uint32 time_us) ICACHE_FLASH_ATTR;

#endif // _INCLUDE_DEEP_SLEEP_H_
