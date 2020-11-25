CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all: libMyPeri.a buzzertest


libMyPeri.a:	 led.o button.o buzzer.o
	$(AR) rc libMyPeri.a led.o button.o buzzer.o

led.o:	led.h led.c
	$(CC) led.c -o led.o -c

buzzer.o:	buzzer.h buzzer.c
	$(CC) buzzer.c -o buzzer.o -c

button.o:	button.h button.c
	$(CC) button.c -o button.o -c -lpthread

buzzertest:	buzzertest.c buzzer.h libMyPeri.a
	$(CC) buzzertest.c -o buzzertest -l MyPeri -L.
	scp buzzertest ecube@192.168.219.148:/home/ecube 
	

clean:
	rm *.o -rf
	rm *.a -rf
	rm ledtest -rf
	rm buzzertest1 -rf
