
CC = gcc
CFLAGS = -MMD -MP

EXECUTABLE = bminor

SRC = $(wildcard *.c)

$(EXECUTABLE): token.h parser.o scanner.o $(SRC:%.c=%.o)
	$(CC) $(CFLAGS) -o $@ $^

test: $(EXECUTABLE)
	sh ./run-tests.sh ./$(EXECUTABLE) -scan ./tests/scanner
	sh ./run-tests.sh ./$(EXECUTABLE) -parse ./tests/parser

scanner.c: scanner.flex token.h
	flex -oscanner.c scanner.flex

parser.c token.h: parser.bison
	bison --graph --defines=token.h --output=parser.c parser.bison

-include $(SRC:%.c=%.d)

clean:
	rm -f $(EXECUTABLE) scanner.c parser.c parser.dot token.h *.o *.d ./tests/scanner/*.out ./tests/parser/*.out
