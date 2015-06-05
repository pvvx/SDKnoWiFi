/******************************************************************************
 * FileName: libmain.h
 * Description: Alternate SDK (libmain.a)
 * Author: PV`
 * (c) PV` 2015
*******************************************************************************/

#ifndef _INCLUDE_LIBMAIN_H_
#define _INCLUDE_LIBMAIN_H_

void read_macaddr_from_otp(uint8 *mac) ICACHE_FLASH_ATTR;
void uart_wait_tx_fifo_empty(void) ICACHE_FLASH_ATTR;

#endif /* _INCLUDE_LIBMAIN_H_ */
