/*
 * No WIFi loader/SDK
 *
 * (c) PV`
 */

#ifndef __USER_INTERFACE_H__
#define __USER_INTERFACE_H__

#include "user_config.h"
#include "os_type.h"
#include "osapi.h"
#include "queue.h"
#include "bios.h"
#include "sys.h"

#ifndef MAC2STR
#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"
#endif

#define system_get_time() (*((volatile unsigned int *)(0x3FF20C00)))
#define system_get_rtc_time  IO_RTC_SLP_CNT_VAL

#define SYS_CPU_80MHZ	80
#define SYS_CPU_160MHZ	160


#endif // __USER_INTERFACE_H__
