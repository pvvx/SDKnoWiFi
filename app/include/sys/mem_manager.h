/******************************************************************************
 * FileName: mem_manager.h
 * Description: mem manager (libmem.a)
 * Alternate SDK
 * Author: PV`
 * (c) PV` 2015
*******************************************************************************/

#ifndef __MEM_H__
#define __MEM_H__

// в модуле mem_manager.o libmem.a

void *pvPortMalloc( size_t xWantedSize );
void vPortFree( void *pv );
void *pvPortZalloc(size_t size);
void *pvPortCalloc(unsigned int n, unsigned int count);
void *pvPortRealloc(void * p, size_t size);
size_t xPortGetFreeHeapSize(void);

#define os_malloc   pvPortMalloc
#define os_free     vPortFree
#define os_zalloc   pvPortZalloc
#define os_calloc   pvPortCalloc
#define os_realloc  pvPortRealloc
#define system_get_free_heap_size xPortGetFreeHeapSize

/* eagle.app.v6.ld */
extern uint8_t _bss_start, _bss_end, _data_start, _data_end, _heap_start, _rodata_start, _rodata_end;


#endif // __MEM_H__

