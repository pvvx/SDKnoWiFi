/******************************************************************************
 * FileName: libmain.h
 * Description: Alternate SDK (libmain.a)
 * Author: PV`
 * (c) PV` 2015
 * ver 0.0.0 (b0)
*******************************************************************************/

#ifndef _INCLUDE_FATAL_ERRS_H_
#define _INCLUDE_FATAL_ERRS_H_


#define RST_EVENT_WDT 1
#define RST_EVENT_RES 8
#define RST_EVENT_EXP 128

#define FATAL_ERR_R6PHY 9 // "register_chipv6_phy"

void fatal_error(uint32_t errn, void *addr, void *txt);

#endif // _INCLUDE_FATAL_ERRS_H_
