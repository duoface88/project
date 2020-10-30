all:final.elf

final.elf:	1.o 2.o main.o
	gcc -o final.elf 1.o 2.o main.o

1.o: 1.c
	gcc -o 1.o -c 1.c

2.o: 2.c
	gcc -o 2.o -c 2.c

main.o: main.c
	gcc -o main.o -c main.c

clean:
	rm -f *.o
	rm -f *.elf
