CC=g++

CFLAGS=-c -msse4.1

INCDIR=-I/opt/intel/ipp/include

LIBDIR=-L/opt/intel/ipp/lib

LIBFILE=-lippcore

all: main

main: main.o 
	$(CC) main.o -o main

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

clean:
	rm -f *o main
