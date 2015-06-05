/******************************************************************************
 * FileName: loader.c
 * Description: Alternate SDK (libmain.a)
 * Author: PV`
 * (c) PV` 2015
*******************************************************************************/
#include "user_config.h"
#include "bios.h"
#include "hw/esp8266.h"
#include "hw/spi_register.h"
#include "sys/loader.h"

//=============================================================================
// extern funcs
//-----------------------------------------------------------------------------
extern void startup(void);
//=============================================================================
// IRAM code
//=============================================================================
// call_user_start() - вызов из заголовка, загрузчиком
// ENTRY(call_user_start) in eagle.app.v6.ld
//-----------------------------------------------------------------------------
void call_user_start(void)
{
	    // Загрзука заголовка flash
	    struct SPIFlashHead fhead;
		SPI0_USER |= SPI_CS_SETUP; // +1 такт перед CS
		SPIRead(0, (uint32_t *)&fhead, sizeof(fhead));
		// Установка размера Flash от 256Kbytes до 32Mbytes
		// High four bits fhead.hsz.flash_size: 0 = 512K, 1 = 256K, 2 = 1M, 3 = 2M, 4 = 4M, ... 7 = 32M
	    uint32 fsize = fhead.hsz.flash_size & 7;
		if(fsize < 2) flashchip->chip_size = (8 >> fsize) << 16;
		else flashchip->chip_size = (4 << fsize) << 16;
	    uint32 fspeed = fhead.hsz.spi_freg;
		// Установка:
		// SPI Flash Interface (0 = QIO, 1 = QOUT, 2 = DIO, 0x3 = DOUT)
		// and Speed QSPI: 0 = 40MHz, 1= 26MHz, 2 = 20MHz, ... = 80MHz
		sflash_something(fspeed);
		// SPIFlashCnfig(fhead.spi_interface & 3, (speed > 2)? 1 : speed + 2);
		// SPIReadModeCnfig(5); // in ROM
		// Всё - включаем кеширование, далее можно вызывать процедуры из flash
		Cache_Read_Enable(0,0,1);
		// Инициализация
		startup();
		// Передача управления ROM-BIOS
		ets_run();
}
//-----------------------------------------------------------------------------
// Установка скорости QSPI
//  0 = 40MHz, 1 = 26MHz, 2 = 20MHz, >2 = 80MHz
//-----------------------------------------------------------------------------
void sflash_something(uint32 flash_speed)
{
	//	Flash QIO80:
	//  SPI_CTRL = 0x16ab000 : QIO_MODE | TWO_BYTE_STATUS_EN | WP_REG | SHARE_BUS | ENABLE_AHB | RESANDRES | FASTRD_MODE | BIT12
	//	IOMUX_BASE = 0x305
	//  Flash QIO40:
	//	SPI_CTRL = 0x16aa101
	//	IOMUX_BASE = 0x205
	uint32 xreg = (SPI0_CTRL >> 12) << 12; //  & 0xFFFFF000
	uint32 value;
	if (flash_speed > 2) { // 80 MHz
		value = BIT(12); // 0x60000208 |= 0x1000
		GPIO_MUX_CFG |= (1<< MUX_SPI0_CLK_BIT); // HWREG(IOMUX_BASE, 0) |= BIT(8);  // 80 MHz
	}
	else { // 0:40, 1:26, 2:20 MHz // 0x101, 0x202, 0x313
		value = 1 + flash_speed + ((flash_speed + 1) << 8) + ((flash_speed >> 1)<< 4);
		xreg &= ~BIT(12);  // 0x60000208 &= 0xffffefff
		GPIO_MUX_CFG &= MUX_CFG_MASK & (~(1<< MUX_SPI0_CLK_BIT)); // HWREG(IOMUX_BASE, 0) &= ~(BIT(8));  // 0x60000800 &=  0xeff
	}
	SPI0_CTRL = xreg | value;
}


