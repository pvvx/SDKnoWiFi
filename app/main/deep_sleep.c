/*
 * deep_sleep.c
 *
 *  Created on: 03 июня 2015 г.
 *      Author: PVV
 */
#include "user_config.h"
#include "bios.h"
#include "hw/esp8266.h"
#include "hw/gpio_register.h"
#include "os_type.h"
#include "user_interface.h"
#include "fatal_errs.h"

uint32 ICACHE_FLASH_ATTR pm_usec2rtc(uint32 us, uint32 cf)
{
	if(cf == 0) {
		cf = 5;
		return us/cf;
	}
	if(us <= 0xFFFFF) {
		us <<= 12;
		return us/cf;
	}
	return (us/cf) << 12;
}

void ICACHE_FLASH_ATTR sys_deep_sleep(uint32 time_us)
{
	os_printf("sys_deep_sleep %u us...\n", time_us);
	uart_wait_tx_fifo_empty();
	ets_delay_us(100);
	IO_RTC_0 = 0;
//	IO_RTC_0 &= ~BIT14;
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

	RTC_CALIB_VALUE  &= ~ RTC_CALIB_RDY;
	RTC_CALIB_SYNC = RTC_CALIB_RDY | 0x101;
	ets_delay_us(100);
	while((RTC_CALIB_VALUE & RTC_CALIB_RDY) == 0);
	uint32 clpr = ((RTC_CALIB_VALUE & 0xFFFFFFF) << 4) / 26; // 26 MHz
	uint32 x = pm_usec2rtc(time_us, clpr); // clpr = ~ 22500
//	os_printf("calib_val = %u, add_RTC = %u, RTC_CALIB_VALUE = 0x%08x\n", clpr, x, RTC_CALIB_VALUE); // = 0?
//	calib_val = 22502, add_RTC = 544768, RTC_CALIB_VALUE = 0xc0008ed7
//	uart_wait_tx_fifo_empty();

	if(x == 0 || clpr == 0) fatal_error(FATAL_ERR_DPSLP, sys_deep_sleep, NULL);
	ets_wdt_disable();
	ets_intr_lock();
	RTC_MEM(0) = RST_EVENT_DEEP_SLEEP;

	IO_RTC_SLP_VAL = IO_RTC_SLP_CNT_VAL + x;

//	RTC_GPI2_CFG &= 0x11;
//	IO_PAD_XPD_DCDC_CONF = 0x03;

	RTC_BASE[16] = 0x7F;
	RTC_BASE[16] = 0x20;
	IO_RTC_4 = 0;

	IO_RTC_6 = 8;
//	Cache_Read_Disable();
	SPI0_CMD = 1<<21;
	while(SPI0_CMD);
	IO_RTC_2 = 1<<20; // rtc_enter_sleep()
	ets_delay_us(100);
	_ResetVector();
}

