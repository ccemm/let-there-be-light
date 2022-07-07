#include <genericTypes.h>
#include <stdint.h>
#include "drvLeds.h"
#include "drvConfig.h"
#include "drvLeds.h"
#include "stm32l100xc.h"
//typedef  void(*ClickCallback)(BtnEvent evt);

int main()
{
  drvLedsInit();
  
  
  while(1)
  {
    drvLedToggle();
  }    
    
    
    return 0;
}

