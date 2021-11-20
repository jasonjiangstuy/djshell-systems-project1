all: shell.o main.o
	gcc -o djshell shell.o main.o

shell.o: shell.c shell.h
	gcc -c shell.c

main.o: main.c
	gcc -c main.c 

clean:
	rm -rf *.o

run:
	./djshell