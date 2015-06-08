/******************************************************************************
 * FileName: delay.c
 * Description: Alternate SDK (libmain.a)
 * Author: PV`
 * (c) PV` 2015
*******************************************************************************/
#include "user_config.h"
#include "bios/ets.h"
#include "sys/fatal_errs.h"
#include "hw/esp8266.h"
#include "sys/delay.h"

void ICACHE_FLASH_ATTR delay_timer0_isr(bool * flag)
{
	*flag = false;
	TIMER0_INT = 0; // ~ TIMER0_INT_CLR_MASK;
}

const char ICACHE_RODATA_ATTR sys_delay_us_err[] = "sys_delay_us > 1677721!";

void ICACHE_FLASH_ATTR sys_delay_us(uint32 us)
{
	if(us < 75) {
		ets_delay_us(us);
		return;
	}
	else if(us > 1677721) {
		fatal_error(FATAL_ERR_DELAY, sys_delay_us, (void *)sys_delay_us_err);
	}
	volatile bool flag_ok = true;
    TIMER0_LOAD = us*5 - 9;
    TIMER0_CTRL = TM_DIVDED_BY_16 | TM_ENABLE_TIMER | TM_EDGE_INT;
    if((INTC_EDGE_EN & BIT(1)) == 0) {
        ets_isr_attach(ETS_FRC_TIMER0_INUM, delay_timer0_isr, (void *) &flag_ok);
        INTC_EDGE_EN |= BIT(1);
        ets_isr_unmask(BIT(ETS_FRC_TIMER0_INUM));
    }
	while(flag_ok)	{
		asm volatile ("waiti 0");
	}
}



