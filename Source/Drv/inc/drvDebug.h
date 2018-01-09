/*
 * drvDebug.h
 *
 *  Created on: 7 Jan 2018
 *  Author: cem
 */

#ifndef DRVDEBUG_H_
#define DRVDEBUG_H_

int drvDebugInit(void);
int drvSetInterface(int iface);
void drvPrintf(const char * format, ...);

#endif /* DRVDEBUG_H_ */
