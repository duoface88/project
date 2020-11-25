CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

<<<<<<< HEAD
all: libMyPeri.a  fndtest1
=======
all: libMyPeri.a fndtest1
>>>>>>> fb1d11d6bccb6ac7f454cc87f653394c7e89d54d

libMyPeri.a:   led.o button.o buzzer.o fnd.o
   $(AR) rc libMyPeri.a led.o button.o buzzer.o fnd.o

<<<<<<< HEAD
led.o:   led.h led.c
   $(CC) led.c -o led.o -c
=======
libMyPeri.a:	 led.o button.o buzzer.o fnd.o
	$(AR) rc libMyPeri.a led.o button.o buzzer.o fnd.o 
>>>>>>> fb1d11d6bccb6ac7f454cc87f653394c7e89d54d

buzzer.o:   buzzer.h buzzer.c
   $(CC) buzzer.c -o buzzer.o -c

button.o:   button.h button.c
   $(CC) button.c -o button.o -c -lpthread

fnd.o:   fnd.h fnd.c
   $(CC) fnd.c -o fnd.o -c

<<<<<<< HEAD
fndtest1: fndtest.c fnd.h libMyPeri.a
   $(CC) fnddtset.c -o fndtest1 -l MyPeri.a

   clean:
      rm *.o -rf
      rm *.a -rf
      rm fndtest1 -rf
=======
fnd.o:	fnd.h fnd.c
	$(CC) fnd.c -o fnd.o -c

fndtest1:	fndtest.c fnd.h libMyPeri.a
	$(CC) fndtest.c -o fndtest1 -l MyPeri -L.
	scp fndtest1 ecube@192.168.219.121:/home/ecube 
	

clean:
	rm *.o -rf
	rm *.a -rf
	rm ledtest -rf
	rm fndtest1 -rf
	rm lcdtexttest1 -rf
>>>>>>> fb1d11d6bccb6ac7f454cc87f653394c7e89d54d
