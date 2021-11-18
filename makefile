all: shell.o main.o
	clang -o djshell main.o shell.o

shell.o: shell.c shell.h
	clang -c shell.c

main.o: main.c
	clang -c main.c 

clean:
	rm -rf *.o

run:
	./djshell