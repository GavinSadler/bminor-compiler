
CC = gcc
CFLAGS = -MMD -MP

EXECUTABLE = bminor

SRC = $(wildcard *.c)

$(EXECUTABLE): $(SRC:%.c=%.o) scanner.o
	$(CC) $(CFLAGS) -o $@ $^

test: $(EXECUTABLE)
	sh ./run-tests.sh ./$(EXECUTABLE) -scan ./tests/scanner

scanner.c: scanner.flex token.h
	flex -oscanner.c scanner.flex

parser.c token.h: parser.bison
	bison --defines=token.h --output=parser.c parser.bison

-include $(SRC:%.c=%.d)

clean:
	rm -f $(EXECUTABLE) scanner.c *.o *.d ./tests/scanner/*.out parser.c token.h
