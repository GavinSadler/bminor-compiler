
CC = gcc
CFLAGS = -MMD -MP

EXECUTABLE = bminor

SRC = $(wildcard *.c)

$(EXECUTABLE): $(SRC:%.c=%.o) scanner.o
	$(CC) $(CFLAGS) -o $@ $^

scanner.c: scanner.flex
	flex -oscanner.c scanner.flex

-include $(SRC:%.c=%.d)

clean:
	rm -f $(EXECUTABLE) scanner.c *.o *.d
