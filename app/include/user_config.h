#ifndef _user_config_h_
#define _user_config_h_

#ifndef ICACHE_FLASH
	#define ICACHE_FLASH
//	#define ICACHE_FLASH_ATTR __attribute__((section(".irom0.text")))
//	#define ICACHE_RODATA_ATTR __attribute__((section(".irom.text")))
#endif


#ifndef USE_OPTIMIZE_PRINTF
	#define USE_OPTIMIZE_PRINTF
#endif

#ifndef USE_US_TIMER
	#define USE_US_TIMER
#endif

#define LOADER_VERSION 0001   // 0001 = 0.0.0 b1

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#define DEBUGSOO	2  // 0 - откл вывода, 1 - минимум, 2 - норма, >3 - текушая отладка, >4 - удалить что найдется :)

//#define USE_CPU_SPEED  160 // 80

#endif // _user_config_h_


