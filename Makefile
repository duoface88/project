CC	=	arm-linux-gnueabi-gcc
AR	=	arm-linux-gnueabi-ar

all:	libMyPeri.a ledtest
	
libMyPeri.a:	led.o
	$(AR) rc libMyPeri.a led.o

led.o:	led.h led.c
	$(CC) led.c -o led.o -c

ledtest:	ledtest.c led.h libMyPeri.a
	$(CC) ledtest.c -o ledtest -l Myperi -L.
	scp  -P30201 ledtest ecube@61.36.233.22:/home/ecube

clean:
		rm *.o -rf
		rm *.a -rf
		rm ledtest -rf