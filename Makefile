all:	final.elf

final.elf:	1.o 2.o
	gcc -o final.elf 1.o 2.o

1.o: 1.c
	gcc -o 1.o 1.c

2.0: 2.c
	gcc -o 2.o 2.c

clean:
	rm -f *.o
	rm -f *.elf