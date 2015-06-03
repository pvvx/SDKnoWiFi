/*
 * startup.h
 *
 *  Created on: 13 мая 2015 г.
 *      Author: PVV
 */

#ifndef _INCLUDE_STARTUP_H_
#define _INCLUDE_STARTUP_H_

#define DEBUG_UART 0 // включить вывод в загрузчике сообщений

void startup(void) ICACHE_FLASH_ATTR;

#endif /* _INCLUDE_STARTUP_H_ */
