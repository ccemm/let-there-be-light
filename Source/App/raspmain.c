#include <stdio.h>
#include <unistd.h>
#include "pigpio.h"
#include "time.h"

// ---- Button Pin 17
// ---- Led Pin 18

void keyStateChanged(int event, int level, uint32_t tick)
{
	static bool ledState=false;
	static int prevTick = 0;
	
	if(event = 17 && level == 0)
	{
		if(ledState)
		{
			ledState =0;
			gpioWrite(18,0);
		}
		else 
		{
			ledState =1;
			gpioWrite(18,1);
		}
	}
}

int main(void)
{
	printf("!!! RPI Program Started !!!\n");
	
	// init pigpio
	if(gpioInitialise() <0)
	{
		printf("Init Failed \n");
		gpioTerminate();
		return -1;
	}
	
	// Set Pin Directions
	if(gpioSetMode(18, PI_OUTPUT) < 0 || 
		gpioSetMode(17, PI_INPUT)<0 )
	{
		printf("Set Mod Failed \n");
		gpioTerminate();
		return -1;
	}
	
	// Activate Pull-Up for Button
	gpioSetPullUpDown(17, PI_PUD_UP); // Sets a pull-down
	// Glitch Filter 100 Ms
	gpioGlitchFilter(17, 100000 /*micoseconds == 100Ms */);
	// Set Call Back For Button Event
	gpioSetAlertFunc(17, keyStateChanged);
		
	while(1)
	{
		sleep(1);
	}
	
	gpioTerminate();
	return 0;
}