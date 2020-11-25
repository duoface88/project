CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

all: libMyPeri.a  fndtest1

libMyPeri.a:   led.o button.o buzzer.o fnd.o
   $(AR) rc libMyPeri.a led.o button.o buzzer.o fnd.o

led.o:   led.h led.c
   $(CC) led.c -o led.o -c

buzzer.o:   buzzer.h buzzer.c
   $(CC) buzzer.c -o buzzer.o -c

button.o:   button.h button.c
   $(CC) button.c -o button.o -c -lpthread

fnd.o:   fnd.h fnd.c
   $(CC) fnd.c -o fnd.o -c

fndtest1: fndtest.c fnd.h libMyPeri.a
   $(CC) fnddtset.c -o fndtest1 -l MyPeri.a

   clean:
      rm *.o -rf
      rm *.a -rf
      rm fndtest1 -rf