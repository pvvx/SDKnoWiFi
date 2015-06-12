/******************************************************************************
 * FileName: deep_sleep.c
 * Description: Alternate SDK (libmain.a)
 * Author: PV`
 * (c) PV` 2015
*******************************************************************************/
#include "system.h"
#include "hw/esp8266.h"
#include "hw/gpio_register.h"


uint32 ICACHE_FLASH_ATTR rtc_calib(uint32 k_xtal_mhz)
{
	RTC_CALIB_SYNC  = 0;
	ets_delay_us(20);
	RTC_CALIB_SYNC = RTC_CALIB_RDY | k_xtal_mhz; // k_xtal_mhz + 1;
	ets_delay_us(20);
	while((RTC_CALIB_VALUE & RTC_CALIB_RDY) == 0); // ~ 2 ms
	return RTC_CALIB_VALUE & 0xFFFFFFF; // The cycle number of clk_xtal (crystal clock) for the RTC_PERIOD_NUM cycles of RTC-clock
}

void ICACHE_FLASH_ATTR go_deep_sleep(uint32 x)
{
	ets_wdt_disable();
	ets_intr_lock();
	RTC_MEM(0) = RST_EVENT_DEEP_SLEEP;

	IO_RTC_SLP_VAL = IO_RTC_SLP_CNT_VAL + x;

//	RTC_GPI2_CFG &= 0x11; // дубль, установленны ранее
//	IO_PAD_XPD_DCDC_CONF = 0x03;

	RTC_BASE[16] = 0x7F;
	RTC_BASE[17] = 0x20;

	IO_RTC_4 = 0; // отключить WiFi ?

	IO_RTC_6 = 0x18;
//	Cache_Read_Disable();
	SPI0_CMD = 1<<21;
	while(SPI0_CMD);
	IO_RTC_2 = 1<<20; // rtc_enter_sleep()
	ets_delay_us(100);
	_ResetVector();

}
// полный sleep, с перезагрузкуой
void ICACHE_FLASH_ATTR sys_deep_sleep(uint32 time_us)
{
	time_us >>= 13;
	if(time_us == 0) time_us = 1;
	os_printf("sys_deep_sleep %u us...\n", time_us << 13);
	uart_wait_tx_fifo_empty();
	ets_delay_us(100);
	IO_RTC_0 = 0;
	IO_RTC_0 |= 0x30;
	RTC_BASE[17] = 4;
	IO_RTC_3 = 0x10010;

	RTC_BASE[18] = (RTC_BASE[18] &  0xFFFF01FF) | 0xFC00;
	RTC_BASE[18] = (RTC_BASE[18] &  0xE00) | 0x80;

	IO_RTC_SLP_VAL = IO_RTC_SLP_CNT_VAL + 136; // +136
	IO_RTC_6 = 8;
	IO_RTC_2 = 1<<20;
	ets_delay_us(200);
	RTC_GPI2_CFG = 0x11;
	IO_PAD_XPD_DCDC_CONF = 0x03;
	IO_RTC_3 = 0x640C8;
	IO_RTC_0 &= 0xFCF;

//	rom_i2c_writeReg(106,2,8,0);

	uint32 clpr = rtc_calibrate(26); // 26 MHz, clpr = ~ 51000
/*	os_printf("calib_val = %u, add_val=%u\n", clpr, (clpr>>5)*time_us);
	uart_wait_tx_fifo_empty();
	ets_delay_us(100); */
	// (time_us/8192)*(rtc_calibrate(f_xtal_clk)/32) = ~19454956
	go_deep_sleep((clpr>>5)*time_us); //  tik ~5.1 us?
}

