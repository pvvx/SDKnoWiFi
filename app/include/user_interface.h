/*
 * No WIFi loader/SDK
 *
 * (c) PV`
 */

#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#include "os_type.h"
#include "osapi.h"
#include "mem_manager.h"
#include "queue.h"
#include "user_config.h"
#include "flash.h"

#ifndef MAC2STR
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
#endif

int os_sprintf_plus(char *str, const char *format, ...) ICACHE_FLASH_ATTR;
int os_printf_plus(const char *format, ...) ICACHE_FLASH_ATTR;
void read_macaddr_from_otp(uint8 *mac) ICACHE_FLASH_ATTR;
void uart_wait_tx_fifo_empty(void) ICACHE_FLASH_ATTR;
void call_user_start(void);

#define system_get_time() (*((volatile unsigned int *)(0x3FF20C00)))
#define system_get_rtc_time  IO_RTC_SLP_CNT_VAL

#define SYS_CPU_80MHZ	80
#define SYS_CPU_160MHZ	160

void fatal_error(uint32_t errn, void *addr, void *txt);
void sys_deep_sleep(uint32 time_us) ICACHE_FLASH_ATTR;


#endif
