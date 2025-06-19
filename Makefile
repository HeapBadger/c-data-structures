# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -Iinclude -Itest/include
LDFLAGS = -lcunit

# Directories
SRC_DIR = src
TEST_SRC_DIR = test/src
OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib

# Source and object files
SRC_SOURCES = $(wildcard $(SRC_DIR)/*.c)
TEST_SOURCES = $(wildcard $(TEST_SRC_DIR)/*.c) test/test_main.c
SRC_OBJECTS = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC_SOURCES:.c=.o))
TEST_OBJECTS = $(patsubst $(TEST_SRC_DIR)/%, $(OBJ_DIR)/%, $(TEST_SOURCES:.c=.o))
TEST_EXEC = $(BIN_DIR)/test_main

.PHONY: all clean format valgrind

# Default target
all: $(OBJ_DIR) $(BIN_DIR) $(SRC_OBJECTS) $(TEST_EXEC)
	rm -rf $(OBJ_DIR)
	rm test/test_main.o
	@echo "Build complete."

# Create object files from src/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create object files from test/src/
$(OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link the test executable
$(TEST_EXEC): $(SRC_OBJECTS) $(TEST_OBJECTS)
	$(CC) $(SRC_OBJECTS) $(TEST_OBJECTS) -o $(TEST_EXEC) $(LDFLAGS) 

# Run tests with Valgrind
valgrind: all
	valgrind --leak-check=full --track-origins=yes ./$(TEST_EXEC)

# Format all C source and header files
format:
	clang-format-15 -i $(SRC_SOURCES) $(wildcard include/*.h) $(TEST_SOURCES) $(wildcard test/include/*.h)
	@echo "Formatting complete."

# Create a custom library
lib: $(OBJS)
	@mkdir -p $(LIB_DIR)
	ar rcs $(LIB_DIR)/libcds.a $(OBJS)

# Create necessary directories
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Clean object and executable files
clean:
	rm -rf test/test_main.o
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)
	rm -rf $(LIB_DIR)
	@echo "Clean complete."
