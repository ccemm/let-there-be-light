
CC=arm-linux-gnueabihf-g++.exe 
CFLAGS=-I.  #./Source/App/inc #-I./
SRC_FILES=./Source/App/raspmain.c

DEPS=hellomake.h

LIB_VERSION=0.1

#%.o: %.c $(DEPS)
#	%CC -c -o $@ $< $(CFLAGS)

rasp: ltbl_rasp

ltbl_rasp: $(SRC_FILES)
		$(CC) -o ltbl $(SRC_FILES) $(CFLAGS)

.PHONY: clean

clean: 	
		rm	-f ./ltbl
		#rm -f ./Source/App/src/*.o
		#rm -f ./Source/App/*.o
		#rm -f ./Drv/RaspberryPi/*.o
		#rm -f ./Drv/Midd/*.o
		#rm *.o
		
		