CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all: libMyPeri.a accelMagGyrotest1


libMyPeri.a:	 led.o button.o buzzer.o fnd.o lcdtext.o colorled.o temp.o accelMagGyro.o
	$(AR) rc libMyPeri.a led.o button.o buzzer.o fnd.o lcdtext.o colorled.o temp.o accelMagGyro.o

led.o:	led.h led.c
	$(CC) led.c -o led.o -c

buzzer.o:	buzzer.h buzzer.c
	$(CC) buzzer.c -o buzzer.o -c

button.o:	button.h button.c
	$(CC) button.c -o button.o -c -lpthread

fnd.o:	fnd.h fnd.c
	$(CC) fnd.c -o fnd.o -c

lcdtext.o:	lcdtext.h lcdtext.c
	$(CC) lcdtext.c -o lcdtext.o -c

colorled.o:	colorled.h colorled.c
	$(CC) colorled.c -o colorled.o -c

temp.o:	temp.h temp.c
	$(CC) temp.c -o temp.o -c

accelMagGyro.o:	accelMagGyro.h accelMagGyro.c
	$(CC) accelMagGyro.c -o accelMagGyro.o -c

accelMagGyrotest1:	accelMagGyrotest.c accelMagGyro.h libMyPeri.a
	$(CC) accelMagGyrotest.c -o accelMagGyrotest1 -l MyPeri -L.
	scp accelMagGyrotest1 ecube@192.168.219.100:/home/ecube 
	
clean:
	rm *.o -rf
	rm *.a -rf
	rm fndtest1 -rf
	rm lcdtexttest1 -rf
	rm colorledtest1 -rf
	rm temptest1 -rf
	rm accelMagGyrotest1 -rf
