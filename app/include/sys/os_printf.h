/******************************************************************************
 * FileName: os_printf.h
 * Description: Alternate SDK (libmain.a)
 * Author: PV`
 * (c) PV` 2015
*******************************************************************************/

#ifndef _INCLUDE_OS_PRINTF_H_
#define _INCLUDE_OS_PRINTF_H_

int os_printf_plus(const char *format, ...) ICACHE_FLASH_ATTR;
void _sprintf_out(char c) ICACHE_FLASH_ATTR;
int os_sprintf_plus(char *str, const char *format, ...) ICACHE_FLASH_ATTR;

#endif /* _INCLUDE_OS_PRINTF_H_ */
