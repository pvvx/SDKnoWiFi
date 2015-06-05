/******************************************************************************
 * FileName: wdt.h
 * Description: Alternate SDK (libmain.a)
 * Author: PV`
 * (c) PV` 2015
*******************************************************************************/

#ifndef _INCLUDE_WDT_H_
#define _INCLUDE_WDT_H_

//#include "sys/ets_sys.h"
//#include "sys/fatal_errs.h"

struct rst_info{
	uint32 flag;
	uint32 exccause;
	uint32 epc1;
	uint32 epc2;
	uint32 epc3;
	uint32 excvaddr;
	uint32 depc;
};

void wdt_feed(void);
void wdt_init(void) ICACHE_FLASH_ATTR;
void wdt_task(ETSEvent *e);
void default_exception_handler(void);
void store_exception_error(uint32_t errn);

void os_print_reset_error(void) ICACHE_FLASH_ATTR;

extern bool wdt_flg;

extern ETSEvent wdt_eventq;

#endif /* _INCLUDE_WDT_H_ */
