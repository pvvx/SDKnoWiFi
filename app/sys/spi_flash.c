/******************************************************************************
 * FileName: spi_flash.c
 * Description: Alternate SDK (libmain.a)
 * Author: PV`
 * (c) PV` 2015
*******************************************************************************/

#include "user_config.h"
#include "bios.h"
#include "hw/esp8266.h"
#include "hw/spi_register.h"
#include "sys/spi_flash.h"

//=============================================================================
// define
//-----------------------------------------------------------------------------
#ifdef USE_MAX_IRAM
#define Cache_Read_Enable_def() Cache_Read_Enable(0, 0, 0)
#else
#define Cache_Read_Enable_def() Cache_Read_Enable(0, 0, 1)
#endif
//-----------------------------------------------------------------------------
#define SPI_FBLK 32
/******************************************************************************
 * FunctionName : spi_flash_read
 * Description  : чтение массива байт из flash
 *  			  читает из flash по QSPI блоками по SPI_FBLK байт
 *  			  в ROM-BIOS SPI_FBLK = 32 байта, 64 - предел SPI буфера
 * Parameters   : flash Addr, pointer, кол-во
 * Returns      : SpiFlashOpResult 0 - ok
 * Опции gcc: -mno-serialize-volatile !
 *******************************************************************************/
SpiFlashOpResult __attribute__((optimize("O3"))) spi_flash_read(uint32 faddr, void *des, uint32 size)
{
#if DEBUGSOO > 5
	ets_printf("fread:%p<-%p[%u]\n", des, faddr, size);
#endif
	if(des == NULL) return SPI_FLASH_RESULT_ERR;
	if(size != 0) {
		faddr <<= 8; faddr >>= 8;
		Cache_Read_Disable();
		Wait_SPI_Idle(flashchip);
		uint32 blksize = (uint32)des & 3;
		if(blksize) {
			blksize = 4 - blksize;
			if(size < blksize) blksize = size;
			SPI0_ADDR = faddr | (blksize << 24);
			SPI0_CMD = SPI_READ;
			size -= blksize;
			faddr += blksize;
			while(SPI0_CMD);
			register uint32 data_buf = SPI0_W0;
			do {
				*(uint8 *)des = data_buf;
				des = (uint8 *)des + 1;
				data_buf >>= 8;
			} while(--blksize);
		}
		while(size) {
			if(size < SPI_FBLK) blksize = size;
			else blksize = SPI_FBLK;
			SPI0_ADDR = faddr | (blksize << 24);
			SPI0_CMD = SPI_READ;
			size -= blksize;
			faddr += blksize;
			while(SPI0_CMD);
			uint32 *srcdw = (uint32 *)(&SPI0_W0);
			while(blksize >> 2) {
				*((uint32 *)des) = *srcdw++;
				des = ((uint32 *)des) + 1;
				blksize -= 4;
			}
			if(blksize) {
				uint32 data_buf = *srcdw;
				do {
					*(uint8 *)des = data_buf;
					des = (uint8 *)des + 1;
					data_buf >>= 8;
				} while(--blksize);
				break;
			}
		}
		Cache_Read_Enable_def();
	}
	return SPI_FLASH_RESULT_OK;
}
/******************************************************************************
 * FunctionName : spi_flash_get_id
 * Returns      : flash id
 *******************************************************************************/
uint32 spi_flash_get_id(void)
{
	Cache_Read_Disable();
	Wait_SPI_Idle(flashchip);
	SPI0_W0 = 0;     // 0x60000240 = 0
	SPI0_CMD = SPI_RDID; // 0x60000200 = 0x10000000
	while (SPI0_CMD);
	uint32_t id = SPI0_W0 & 0xffffff;
	Cache_Read_Enable_def();
	return id;
}
/******************************************************************************
 * FunctionName : spi_flash_read_status
 * Returns      : SpiFlashOpResult
 *******************************************************************************/
SpiFlashOpResult spi_flash_read_status(uint32_t * sta)
{
	Cache_Read_Disable();
	uint32 ret = SPI_read_status(flashchip, sta);
	Cache_Read_Enable_def();
	return ret;
}
/******************************************************************************
 * FunctionName : spi_flash_write_status
 * Returns      : SpiFlashOpResult
 *******************************************************************************/
SpiFlashOpResult spi_flash_write_status(uint32_t sta)
{
	Cache_Read_Disable();
	SpiFlashOpResult ret = SPI_write_status(flashchip, sta);
	Cache_Read_Enable_def();
	return ret;
}
/******************************************************************************
 * FunctionName : spi_flash_erase_sector
 * Returns      : SpiFlashOpResult
 *******************************************************************************/
SpiFlashOpResult spi_flash_erase_sector(uint16 sec)
{
	Cache_Read_Disable();
	open_16m();
	SpiFlashOpResult ret = SPIEraseSector(sec);
	close_16m();
	Cache_Read_Enable_def();
	return ret;
}
/******************************************************************************
 * FunctionName : spi_flash_write
 * Returns      : SpiFlashOpResult
 *******************************************************************************/
SpiFlashOpResult spi_flash_write(uint32 des_addr, uint32 *src_addr, uint32 size)
{
	if(src_addr == NULL) return SPI_FLASH_RESULT_ERR;
	if(size & 3) size &= ~3;
	Cache_Read_Disable();
	open_16m();
	SpiFlashOpResult ret = SPIWrite(des_addr, (uint32_t *) src_addr, size);
	close_16m();
	Cache_Read_Enable_def();
	return ret;
}
/******************************************************************************
 * FunctionName : spi_flash_erase_block
 * Returns      : SpiFlashOpResult
 *******************************************************************************/
SpiFlashOpResult spi_flash_erase_block(uint32 blk)
{
	Cache_Read_Disable();
	open_16m();
	SpiFlashOpResult ret = SPIEraseBlock(blk);
	close_16m();
	Cache_Read_Enable_def();
	return ret;
}
/******************************************************************************
 * FunctionName : spi_flash_real_size
 * Returns      : real flash size (512k, 1M, 2M, 4M, 8M, 16M)
 *******************************************************************************/
uint32 ICACHE_FLASH_ATTR spi_flash_real_size(void) {
	uint32 size = FLASH_MIN_SIZE;
	uint32 x1[8], x2[8];
	if (spi_flash_read(0, x1, 8*4) == SPI_FLASH_RESULT_OK) {
		for (size = FLASH_MIN_SIZE; size < FLASH_MAX_SIZE; size <<= 1) {
			if (spi_flash_read(size, x2, 8*4) != SPI_FLASH_RESULT_OK)	break;
			else if (!ets_memcmp(x1, x2, 8*4)) break;
		};
	};
	return size;
}



