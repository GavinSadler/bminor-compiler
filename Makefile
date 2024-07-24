
# Makefile structure based on https://makefiletutorial.com/#makefile-cookbook
# Updated to include correct directives for bison/flex 

CC := gcc

TARGET_EXEC := bminor

BUILD_DIR := ./build
SRC_DIRS := ./src

SRCS := $(shell find $(SRC_DIRS) -name '*.c' ! -name 'scanner.c' ! -name 'parser.c') $(SRC_DIRS)/scanner.c $(SRC_DIRS)/parser.c
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CFLAGS := $(INC_FLAGS) -MMD -MP -Wextra -Wall -Wpedantic -Wshadow -Wvla -g -Og # -Wstrict-prototypes -Wold-style-definition 

# The final build step.
$(TARGET_EXEC):$(SRC_DIRS)/token.h $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Build step for flex scanner
$(SRC_DIRS)/scanner.c: $(SRC_DIRS)/scanner.flex $(SRC_DIRS)/token.h
	@echo "=== COMPILING FLEX SCANNER ==="
	flex -o $(SRC_DIRS)/scanner.c $(SRC_DIRS)/scanner.flex
	@echo "=== FLEX SCANNER COMPLETE ==="

# Build step for bison parser
$(SRC_DIRS)/token.h $(SRC_DIRS)/parser.c: $(SRC_DIRS)/parser.bison
	@echo "=== COMPILING BISON PARSER ==="
	bison --defines=$(SRC_DIRS)/token.h --output=$(SRC_DIRS)/parser.c $(SRC_DIRS)/parser.bison
	@echo "=== BISON PARSER COMPILED ==="

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TARGET_EXEC) $(SRC_DIRS)/scanner.c $(SRC_DIRS)/parser.c $(SRC_DIRS)/token.h
	rm -rf $(SRC_DIRS)/parser.dot $(SRC_DIRS)/parser.gv ./tests/scanner/*.gv ./tests/parser/*.gv ./tests/printer/*.gv ./tests/typecheck/*.gv
	rm -rf ./tests/scanner/*.out ./tests/parser/*.out ./tests/printer/*.out ./tests/typecheck/*.out

.PHONY: format
format:
	clang-format --style=file --verbose -i $(SRC_DIRS)/*.c $(SRC_DIRS)/*.h

.PHONY: test
test: $(TARGET_EXEC)
	@echo "=== TESTING SCANNER ==="
	sh ./run-tests.sh ./$(TARGET_EXEC) --scan ./tests/scanner
	@echo "=== TESTING PARSER ==="
	sh ./run-tests.sh ./$(TARGET_EXEC) --parse ./tests/parser
	@echo "=== TESTING PRINTER IDEMPOTENT ==="
	sh ./tests/printer/printer-idempotent.sh
	@echo "=== TESTING TYPECHECKING ==="
	sh ./run-tests.sh ./$(TARGET_EXEC) --typecheck ./tests/typecheck

.PHONY: graph
graph: $(TARGET_EXEC)
	sh ./graph.sh

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)
