
CC = gcc
CFLAGS = -MMD -MP

EXECUTABLE = bminor

SRC = $(wildcard *.c)

$(EXECUTABLE): token.h parser.o scanner.o $(SRC:%.c=%.o)
	$(CC) $(CFLAGS) -o $@ $^

test: $(EXECUTABLE)
	sh ./run-tests.sh ./$(EXECUTABLE) -scan ./tests/scanner

scanner.c: scanner.flex token.h
	flex --debug -oscanner.c scanner.flex

parser.c token.h: parser.bison
	bison --debug --graph --defines=token.h --output=parser.c parser.bison

-include $(SRC:%.c=%.d)

clean:
	rm -f $(EXECUTABLE) scanner.c parser.c token.h *.o *.d ./tests/scanner/*.out
