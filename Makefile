# Variables
CC = gcc
INCLUDE_DIR = include
CFLAGS = -Wall -Wextra $(shell pkg-config --cflags check) -I$(INCLUDE_DIR)
LDFLAGS = $(shell pkg-config --libs check)
TARGET = build/void-mapper
MAIN_SRC = main.c $(wildcard src/*.c)
MAIN_OBJ = $(patsubst %.c, build/%.o, $(MAIN_SRC))
TEST_SRC = $(wildcard tests/*.c)
TEST_OBJ = $(patsubst %.c, build/%.o, $(TEST_SRC) $(filter-out build/main.o, $(MAIN_OBJ)))
TEST_TARGET = build/test_runner

# Default target
all: $(TARGET)

# Build the main target
$(TARGET): $(MAIN_OBJ)
	@$(CC) $(CFLAGS) -o $@ $^

# Build the test runner
$(TEST_TARGET): $(TEST_OBJ)
	@$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Run unit tests
check: $(TEST_TARGET)
	@./$(TEST_TARGET)

# Clean generated files
clean:
	@rm -rf build

# Pattern rule for object files
build/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all check clean
