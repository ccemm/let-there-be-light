/*******************************************************************
 *
 * $Author: Cem AKIN
 * $Date:
 * $Description:
 *
********************************************************************/
#ifndef _DRV_CONFIG_H
#define _DRV_CONFIG_H

#include "genericTypes.h"

#if !ST_PLATFORM

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#endif

#if PART_TM4C123GH6PM
#include "hw_memmap.h"
#include "tm4c123gh6pm.h"
#include "sysctl.h"
#endif

#define UART_BUFFER_SIZE		1024		// UART Rcv and Transmit Buffer Size
#define RING_BUFFER_MAX  		2		// Max Ring Buffer Number

typedef enum _LedBlinkMode
{
	LED_BLNK_NONE =0,
	LED_BLNK_21_22=1,	// 2 times One Second, 2 times 2 second
	LED_BLNK_41_42=2,	// 4 times One Second, 4 times 2 second
	LED_BLNK_NUM

}LedBlinkMode;


#endif
