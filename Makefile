
CC = gcc
CFLAGS = -MMD -MP

EXECUTABLE = bminor

SRC = $(wildcard *.c)

$(EXECUTABLE): $(SRC:%.c=%.o) scanner.o
	$(CC) $(CFLAGS) -o $@ $^

test: $(EXECUTABLE)
	sh ./run-tests.sh ./bminor -scan ./tests/scanner

scanner.c: scanner.flex
	flex -oscanner.c scanner.flex

-include $(SRC:%.c=%.d)

clean:
	rm -f $(EXECUTABLE) scanner.c *.o *.d ./tests/scanner/*.out
