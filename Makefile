
CC = gcc
CFLAGS = -MMD -MP

EXECUTABLE = bminor

SRC = $(wildcard *.c)

$(EXECUTABLE): $(SRC:%.c=%.o) scanner.o
	$(CC) $(CFLAGS) -o $@ $^

-include $(SRC:%.c=%.d)

scanner.c: scanner.flex
	flex -oscanner.c scanner.flex

clean:
	rm -f $(EXECUTABLE) scanner.c *.o *.d
