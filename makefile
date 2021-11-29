all: parsing.o runs.o shell.o main.o
	gcc -lncurses -o djshell main.o shell.o runs.o parsing.o

parsing.o: parsing.c parsing.h includes.h
	gcc -c parsing.c

runs.o: runs.c runs.h
	gcc -c runs.c

shell.o: shell.c shell.h
	gcc -c shell.c

main.o: main.c
	gcc -c main.c

clean:
	rm -rf *.o

run:
	./djshell
