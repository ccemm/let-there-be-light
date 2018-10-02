
CC=arm-linux-gnueabihf-g++.exe 
CFLAGS=-Wall -pthread -I. -I./Source/Drv/inc/ #./Source/App/inc #-I./
SRC_FILES=./Source/App/raspmain.c ./Source/Drv/RaspberryPi/drvDebug.c ./Source/Drv/RaspberryPi/drvLeds.c ./Source/Drv/RaspberryPi/drvRingBuffer.c   ./Source/Drv/RaspberryPi/drvPushButtons.c
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
		
		
