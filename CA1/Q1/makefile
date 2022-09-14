CC=g++

CFLAGS=-c
INCDIR=-I.

all: main

main: main.o 
	$(CC) main.o -o main

main.o: main.c
	$(CC) $(CFLAGS) $(INCDIR) main.c

clean:
	rm -f *o main