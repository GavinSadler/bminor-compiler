
CC = gcc
EXECUTABLE = bminor

$(EXECUTABLE): main.o scanner.o
	$(CC) main.o scanner.o -o scan	

main.o: main.c token.h
	$(CC) main.c -c -o main.o

scanner.o: scanner.c token.h
	$(CC) scanner.c -c -o scanner.o

scanner.c: scanner.flex
	flex -oscanner.c scanner.flex

clean:
	rm -f scanner.c scanner.o main.o bminor
