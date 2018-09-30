#include <stdio.h>
#include <unistd.h>
#include "pigpio.h"
#include "time.h"

int main(void)
{
	printf("!!! RPI Program Started !!!\n");
	
	if(gpioInitialise() <0)
	{
		printf("Init Failed \n");
		gpioTerminate();
		return -1;
	}
	
	if(gpioSetMode(18, PI_OUTPUT) < 0)
	{
		printf("Set Mod Failed \n");
		//gpioSetPullUpDown(18, PI_PUD_DOWN); // Sets a pull-down
		gpioTerminate();
		return -1;
	}
	int val=1;
	
	for(int i =0; i<50; i++)
	{
		sleep(1);
		gpioWrite(18, val);
		val = (!val);
	} 
	
	
	gpioTerminate();
	return 0;
}