/*
 * deep_sleep.c
 *
 *  Created on: 03 θώνÿ 2015 γ.
 *      Author: PVV
 */
#include "user_config.h"
#include "bios.h"
#include "hw/esp8266.h"
#include "hw/specreg.h"
#include "hw/gpio_register.h"
//#include "add_sdk_func.h"
#include "os_type.h"
#include "user_interface.h"

#if 0

void _sys_deep_sleep_timer(void *timer_arg)
{
	os_printf_plus("deep sleep %ds\n\n", timer_arg/1000000);
	deep_sleep_set_option(deep_sleep_option);
//	while(READ_PERI_REG(UART_STATUS(0))  & (UART_TXFIFO_CNT<<UART_TXFIFO_CNT_S));
//	while(READ_PERI_REG(UART_STATUS(1))  & (UART_TXFIFO_CNT<<UART_TXFIFO_CNT_S));
	user_uart_wait_tx_fifo_empty(0, 500000);
	user_uart_wait_tx_fifo_empty(1, 500000);

	IO_RTC_0 = 0;
	IO_RTC_0 &= ~BIT14;
	IO_RTC_0 |= 0x30;
	RTC_BASE[17] = 4; //0x60000744 = 4
	IO_RTC_3 = 0x10010; // 	HWREG(PERIPHS_RTC_BASEADDR, 0x0C) = 0x10010;
	RTC_BASE[18] = (RTC_BASE[18] &  0xFFFF01FF) | 0xFC00; // HWREG(PERIPHS_RTC_BASEADDR, 0x48) = (HWREG(PERIPHS_RTC_BASEADDR,0x48) & 0xFFFF01FF) | 0xFC00;
	RTC_BASE[18] = (RTC_BASE[18] &  0xE00) | 0x80; // HWREG(PERIPHS_RTC_BASEADDR, 0x48) = (HWREG(PERIPHS_RTC_BASEADDR, 0x48) & 0xE00) | 0x80;
	IO_RTC_SLP_VAL = IO_RTC_SLP_CNT_VAL + 136; //	HWREG(PERIPHS_RTC_BASEADDR, 0x04) = HWREG(PERIPHS_RTC_BASEADDR, 0x1C) + 0x88;
	IO_RTC_6 = 8; // HWREG(PERIPHS_RTC_BASEADDR, 0x18) = 8;
	IO_RTC_2 = 0x100000; // HWREG(PERIPHS_RTC_BASEADDR, 0x08) = 0x100000;
	ets_delay_us(200);
	RTC_GPI2_CFG = 0x11; //	HWREG(PERIPHS_RTC_BASEADDR, 0x9C) = 0x11;
	IO_PAD_XPD_DCDC_CONF = 0x03; // HWREG(PERIPHS_RTC_BASEADDR, 0xA0) = 0x03;
	IO_RTC_3 = 0x640C8; // HWREG(PERIPHS_RTC_BASEADDR, 0x0C) = 0x640C8;
	IO_RTC_0 &= 0xFCF; // HWREG(PERIPHS_RTC_BASEADDR, 0x00) &= 0xFCF;
	uint32 clpr = pm_rtc_clock_cali_proc();
	pm_set_sleep_time(timer_arg);
	RTC_GPI2_CFG &= 0x11; //	HWREG(PERIPHS_RTC_BASEADDR, 0x9C) &= 0x11;
	IO_PAD_XPD_DCDC_CONF = 0x03; // HWREG(PERIPHS_RTC_BASEADDR, 0xA0) = 0x03;

	INTC_EDGE_EN &= 0x7E; // HWREG(PERIPHS_DPORT_BASEADDR, 4) &= 0x7E; // WDT int off
	ets_isr_mask(1<<8); // Disable WDT isr

	RTC_BASE[16] = 0x7F; // HWREG(PERIPHS_RTC_BASEADDR, 0x40) = 0x7F;
	RTC_BASE[16] = 0x20; // HWREG(PERIPHS_RTC_BASEADDR, 0x44) = 0x20;
	IO_RTC_4 = 0; // HWREG(PERIPHS_RTC_BASEADDR, 0x10) = 0x00;

	if(clpr == 0) {
		IO_RTC_6 = 0; //	HWREG(PERIPHS_RTC_BASEADDR, 0x18) = 0x00;
	}
	else {
		IO_RTC_6 = 8;	//	HWREG(PERIPHS_RTC_BASEADDR, 0x18) = 0x08;
	}
	ets_intr_lock();
	Cache_Read_Disable();
	SPI0_CMD = 0x200000;
	while(SPI0_CMD);
	struct rst_info rst_info;
	system_rtc_mem_read(0, rst_info, sizeof(rst_info));
	ets_memset(rst_info, 0, sizeof(rst_info));
	system_rtc_mem_write(0, &rst_info, sizeof(rst_info));
	IO_RTC_2 = 1<<20; // rtc_enter_sleep()	HWREG(PERIPHS_RTC_BASEADDR, 0x08) = 0x100000;
}

#endif
