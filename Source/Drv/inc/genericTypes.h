/*******************************************************************
 *
 * $Author: Cem AKIN
 * $Date: Kick Start 24.09.2016
 * $Description:
 *
********************************************************************/

#ifndef _GENERIC_TYPES_H
#define _GENERIC_TYPES_H

#define ST_PLATFORM    1

#if !ST_PLATFORM

#include "stdint.h"

#endif 

#define BIT_GET(x,y)    (x&(1<<y))>>y
#define BIT_SET(x,y)    {x=(x|(1<<y));}
#define BIT_RESET(x,y)  {x=(x&((1<<y));}



#define S32				int
#define U32				unsigned int
#define U8				unsigned char
#define S8				char
#define BOOL			unsigned char
#define TRUE			(BOOL)(1)
#define FALSE			(BOOL)(0)
#define NULL			(0)

#define SUCCESS			(S32)(1)
#define FAILURE			(S32)(-1)


#endif
