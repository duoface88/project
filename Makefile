all:	final.elf

final.elf:	1.o 2.o main.o
	gcc -o final.elf 1.o 2.o main.o

1.o: 1.c
	gcc -c -o 1.o 1.c

2.o: 2.c
	gcc -c  -o 2.o 2.c

main.o: main.c
	gcc -o main.o main.c

clean:
	rm -f *.o
	rm -f *.elf
