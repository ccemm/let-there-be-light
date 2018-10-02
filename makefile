
CC=arm-linux-gnueabihf-g++.exe 
CFLAGS=-Wall -pthread -I. -I./Source/Drv/inc/ -I./Source/Midd/inc/ -I./Source/App/inc #-I./
SRC_FILES=./Source/App/main.c ./Source/Drv/RaspberryPi/drvPlatForm.c ./Source/Drv/RaspberryPi/drvDebug.c ./Source/Drv/RaspberryPi/drvLeds.c ./Source/Drv/RaspberryPi/drvRingBuffer.c ./Source/Drv/RaspberryPi/drvPushButtons.c ./Source/Drv/RaspberryPi/drvTimers.c ./Source/Drv/RaspberryPi/drvUART.c ./Source/App/src/appStateIdle.c  ./Source/Midd/middStateMngr.c   

LDIR=../Libs/rasplibs

DEPS=hellomake.h
LIBS=-lpigpio -lrt

LIB_VERSION=0.1

#%.o: %.c $(DEPS)
#	%CC -c -o $@ $< $(CFLAGS)

rasp: ltbl_rasp

ltbl_rasp: $(SRC_FILES)
		$(CC) -o ltbl $(SRC_FILES) $(CFLAGS) $(LIBS)

.PHONY: clean

clean: 	
		rm	-f ./ltbl
		
		
