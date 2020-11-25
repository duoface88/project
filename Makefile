CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all: libMyPeri.a buttontest1


libMyPeri.a : led.o button.o
	$(AR) rc libMyPeri.a led.o button.o 

led.o :	led.h led.c
	$(CC) led.c -o led.o -c
 

button.o :	button.h button.c
	$(CC) button.c -o button.o -c -lpthread

buttontest1 :	buttontest.c button.h libMyPeri.a
	$(CC) buttontest.c -o buttontest1 -l MyPeri -L. -lpthread
	scp buttontest1 ecube@192.168.219.176:/home/ecube 
	

clean:
	rm *.o -rf
	rm *.a -rf
	rm ledtest -rf
	rm buttontest1 -rf
