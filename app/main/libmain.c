/******************************************************************************
 * FileName: libmain.c
 * Description: Alternate SDK (libmain.a)
 * Author: PV`
 * (c) PV` 2015
 * ver 0.0.1
*******************************************************************************/
#include "libmain.h"
#include "bios.h"
#include "hw/esp8266.h"
#include "hw/uart_register.h"

//=============================================================================
// FLASH code (ICACHE_FLASH_ATTR)
//=============================================================================
// Чтение MAC из OTP
//-----------------------------------------------------------------------------
void ICACHE_FLASH_ATTR read_macaddr_from_otp(uint8 *mac)
{
	mac[0] = 0x18;
	mac[1] = 0xfe;
	mac[2] = 0x34;
	if ((!(OTP_CHIPID & (1 << 15)))||((OTP_MAC0 == 0) && (OTP_MAC1 == 0))) {
		mac[3] = 0x18;
		mac[4] = 0xfe;
		mac[5] = 0x34;
	}
	else {
		mac[3] = (OTP_MAC1 >> 8) & 0xff;
		mac[4] = OTP_MAC1 & 0xff;
		mac[5] = (OTP_MAC0 >> 24) & 0xff;
	}
}
//=============================================================================
// uart_wait_tx_fifo_empty()
//-----------------------------------------------------------------------------
void ICACHE_FLASH_ATTR uart_wait_tx_fifo_empty(void)
{
	while((UART0_STATUS >> UART_TXFIFO_CNT_S) & UART_TXFIFO_CNT);
	while((UART1_STATUS >> UART_TXFIFO_CNT_S) & UART_TXFIFO_CNT);
}
