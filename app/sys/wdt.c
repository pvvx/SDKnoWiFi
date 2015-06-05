/******************************************************************************
 * FileName: wdt.c
 * Description: Alternate SDK (libmain.a)
 * Author: PV`
 * (c) PV` 2015
*******************************************************************************/

#include "system.h"
#include "hw/esp8266.h"
#include "hw/specreg.h"

#define WDT_TASK_PRIO 0x1e
bool wdt_flg;

ETSEvent wdt_eventq;

void store_exception_error(uint32_t errn)
{
		uint32_t *ptr = (uint32_t *)(RTC_MEM_BASE);
			*ptr++ = errn;
			*ptr++ = RSR(EXCCAUSE);
			*ptr++ = RSR(EPC1);
			*ptr++ = RSR(EPC2);
			*ptr++ = RSR(EPC3);
			*ptr++ = RSR(EXCVADDR);
			*ptr++ = RSR(DEPC);
		if(errn > RST_EVENT_WDT) _ResetVector();
}

// каждые 1680403 us
void wdt_feed(void)
{
	if (RTC_MEM(0) <= RST_EVENT_WDT) {
		store_exception_error(RST_EVENT_WDT);
		wdt_flg = true;
		ets_post(WDT_TASK_PRIO, 0, 0);
	}
}

void ICACHE_FLASH_ATTR wdt_task(ETSEvent *e)
{
	ets_intr_lock();
	if(wdt_flg) {
		wdt_flg = false;
		WDT_FEED = WDT_FEED_MAGIC;
		if (RTC_MEM(0) <= RST_EVENT_WDT) RTC_MEM(0) = 0;
	}
	ets_intr_unlock();
}

void ICACHE_FLASH_ATTR wdt_init(void)
{
//	RTC_MEM(0) = 0;
	ets_task(wdt_task, WDT_TASK_PRIO, &wdt_eventq, 1);
	ets_isr_attach(ETS_WDT_INUM , wdt_feed, NULL);
	INTC_EDGE_EN |= 1; // 0x3ff00004 |= 1
	ets_wdt_enable(2,3,3); // mode 2 (wdt isr), step 1680403 us
}

void default_exception_handler(void)
{
	store_exception_error(RST_EVENT_EXP);
}

void fatal_error(uint32_t errn, void *addr, void *txt)
{
		uint32_t *ptr = (uint32_t *)(RTC_MEM_BASE);
			*ptr++ = errn;
			*ptr++ = (uint32_t)addr;
			*ptr++ = (uint32_t)txt;
		_ResetVector();
}

//RAM_BIOS:3FFFD814 aFatalException .ascii "Fatal exception (%d): \n"
#define aFatalException ((const char *)(0x3FFFD814))
//RAM_BIOS:3FFFD7CC aEpc10x08xEpc20 .ascii "epc1=0x%08x, epc2=0x%08x, epc3=0x%08x, excvaddr=0x%08x, depc=0x%08x\n"
#define aEpc10x08xEpc20 ((const char *)(0x3FFFD7CC))
//RAM_BIOS:3FFFD870 aNull .ascii "<null>"
#define aNull ((char *)(0x3FFFD870))

void ICACHE_FLASH_ATTR os_print_reset_error(void)
{
	struct rst_info * rst_info = (struct rst_info *)(&RTC_MEM(0));
	if(rst_info->flag >= RST_EVENT_WDT && rst_info->flag <= RST_EVENT_MAX) {
		os_printf("Old reset: ");
		switch(rst_info->flag) {
		case RST_EVENT_WDT:
			os_printf("WDT (%d):\n", rst_info->exccause);
			os_printf_plus((const char *)aEpc10x08xEpc20, rst_info->epc1, rst_info->epc2, rst_info->epc3, rst_info->excvaddr, rst_info->depc);
			break;
		case RST_EVENT_EXP:
			os_printf_plus((const char *)aFatalException, rst_info->exccause);
			os_printf_plus((const char *)aEpc10x08xEpc20, rst_info->epc1, rst_info->epc2, rst_info->epc3, rst_info->excvaddr, rst_info->depc);
			break;
		case RST_EVENT_SOFT_RESET:
			os_printf("SoftReset\n");
			break;
		case RST_EVENT_DEEP_SLEEP:
			os_printf("Deep_Sleep\n");
			break;
		default: {
			char * txt = (char *)rst_info->epc1;
			if(txt == NULL) txt = aNull;
			os_printf("Error (%u): addr=0x%08x, ", rst_info->flag, rst_info->exccause);
			os_printf_plus(txt);
			os_printf("\n");
			}
		}
	}
	rst_info->flag = 0;
}
