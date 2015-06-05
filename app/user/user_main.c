/******************************************************************************
*******************************************************************************/

#include "system.h"


void ICACHE_FLASH_ATTR user_init(void)
{
	char buf[32];
	os_memset(buf, 0, sizeof(buf));
	os_sprintf(buf, "Test 'SDK no WiFi'\n");
	os_printf("%sHeap size:\t%u bytes\nBss size:\t%u bytes\nStack pointer:\t%p\n", buf, xPortGetFreeHeapSize(), &_bss_end - &_bss_start, &buf[32] );
/*	uint32 t1 = system_get_time();
	ets_delay_us(1000);
	uint32 t2 = system_get_time();
	os_printf("ets_delay_us(1000) dt=%u\n", t2-t1);
	t1 = system_get_time();
	ets_delay_us(10000);
	t2 = system_get_time();
	os_printf("ets_delay_us(10000) dt=%u\n", t2-t1);
	t1 = system_get_time();
	ets_delay_us(100000);
	t2 = system_get_time();
	os_printf("ets_delay_us(100000) dt=%u\n", t2-t1); */
	sys_deep_sleep(50000);
}
