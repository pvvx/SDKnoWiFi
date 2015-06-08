/******************************************************************************
*******************************************************************************/

#include "system.h"

#define UART_BAUD 115200

uint32 t1;

void ICACHE_FLASH_ATTR restore_clkfreq(void)
{
	rom_i2c_writeReg(103,4,1,136);
	rom_i2c_writeReg(103,4,2,145);
	UART0_CLKDIV = CALK_UART_CLKDIV(UART_BAUD);
	ets_delay_us(150);
}

void ICACHE_FLASH_ATTR test_cb1(void)
{
	t1 = system_get_time();
	os_printf("Test cb1, t=%u\n", t1);
	uart_wait_tx_fifo_empty();
	ets_delay_us(150);
}

void ICACHE_FLASH_ATTR test_cb2(void)
{
	uint32 t = system_get_time();
	restore_clkfreq();
	os_printf("Test cb2, t=%u, dt=%u\n", t, t-t1);
	uart_wait_tx_fifo_empty();
	ets_delay_us(150);
}

void ICACHE_FLASH_ATTR user_init(void)
{
	char buf[32];
	uart_wait_tx_fifo_empty();
	UART0_CLKDIV = CALK_UART_CLKDIV(UART_BAUD);
	ets_delay_us(200);
	os_memset(buf, 0, sizeof(buf));
	// Test os_sprintf(flash string) ;
	os_sprintf(buf, "\nTest 'SDK no WiFi'\n");
	os_printf("%sHeap size:\t%u bytes\nBss size:\t%u bytes\nStack pointer:\t%p\n", buf, xPortGetFreeHeapSize(), &_bss_end - &_bss_start, &buf[32] );

	// Test sys_delay_us() (пока тест, ещё не доделано до конца - связь с task, wdt и т.д)

	uint32 t1, t2;
	int i = 1000000;
	do {
		t1 = system_get_time();
		sys_delay_us(i);
		t2 = system_get_time();
		os_printf("sys_delay_us(%u), dt=%u\n", i, t2-t1);
		i-=100000;
//		if(i==500000) IO_RTC_4 = 0;
		WDT_FEED = WDT_FEED_MAGIC;
	} while(i);

	// Для сравнения потребления с ets_delay_us

	t1 = system_get_time();
	ets_delay_us(1000000);
	t2 = system_get_time();
	WDT_FEED = WDT_FEED_MAGIC;
	os_printf("ets_delay_us(1000000) dt=%u\n", t2-t1);
	t1 = system_get_time();
	ets_delay_us(1000000);
	t2 = system_get_time();
	WDT_FEED = WDT_FEED_MAGIC;
	os_printf("ets_delay_us(1000000) dt=%u\n", t2-t1);
	t1 = system_get_time();
	ets_delay_us(1000000);
	t2 = system_get_time();
	WDT_FEED = WDT_FEED_MAGIC;
	os_printf("ets_delay_us(1000000) dt=%u\n", t2-t1);

//	ets_set_idle_cb(user_main_cb, NULL);

// Test RTC isr, sleep, ... (пока тест, ещё не адаптировано)

	dtm_params_init(test_cb1, test_cb2); // назначить функцию on_wait_func_cb() до входа в sleep в ets_run() и функцию off_wait_func_cb() после выхода из sleep в ets_run()
	ets_rtc_int_register(); // установить прерывание rtc - rtc_intr_handler() для sleep
	t1 = 7;
	t2 = 1000; // 1 sec
	os_printf("Test sleep (low power) %u ms, %u cycles \n", t2, t1);
	dtm_set_params(0, t2, 0, t1, 0); // параметры sleep
//	rtc_enter_sleep(); // исполнить один цикл тут
	ets_enter_sleep(); // назначить исполнение sleep в ets_run()
	os_printf("go ets_run()\n");

// Test deep_sleep
//	sys_deep_sleep(50000); // полный sleep, с перезагрузкуой.
} // go ets_run()
