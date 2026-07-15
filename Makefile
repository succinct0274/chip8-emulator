# ==========================================
# Compiler and Flags Configuration
# ==========================================
CC       := clang
CFLAGS   := -Wall -Wextra -std=c11 -O2 $(shell pkg-config sdl3 --cflags)
LDFLAGS  := $(shell pkg-config sdl3 --libs) -lm

# ==========================================
# Directory Layout
# ==========================================
# If your files are in the root directory, leave these as .
# If you use subfolders, update them here (e.g., SRC_DIR := src)
SRC_DIR  := src
LIB_DIR  := lib
INC_DIR  := include
OBJ_DIR  := obj
BIN_DIR  := bin
TEST_DIR := tests

TARGET   := $(BIN_DIR)/chip8
CPPFLAGS := -I$(INC_DIR)

TEST_BIN = bin/tests
TEST_SRCS = $(shell find $(TEST_DIR) -name "*.c")
TEST_BINS = $(patsubst $(TEST_DIR)/%.c, $(TEST_BIN)/%, $(TEST_SRCS))

# ==========================================
# Source and Object File Detection
# ==========================================
APP_SRC := $(SRC_DIR)/main.c
LIB_SRCS := $(wildcard $(LIB_DIR)/*.c)

# SRCS := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))
APP_OBJ := $(OBJ_DIR)/main.o
LIB_OBJS := $(patsubst $(LIB_DIR)/%.c, $(OBJ_DIR)/%.o, $(LIB_SRCS))
ALL_OBJS := $(APP_OBJ) $(LIB_OBJS)
# OBJS := $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))

# ==========================================
# Build Rules
# ==========================================
.PHONY: all clean run debug test tests

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(ALL_OBJS) | $(BIN_DIR)
	$(CC) $(ALL_OBJS) -o $@ $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: lib/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Create directories if they don't exist
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

$(TEST_BIN)/%: $(TEST_DIR)/%.c $(LIB_OBJS) | $(BIN_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< $(LIB_OBJS) -o $@ $(LDFLAGS)

# Run the emulator
run: all
	./$(TARGET) ./roms/chipwar.ch8

test: $(TEST_BINS)
	@for test in $(TEST_BINS); do ./$$test; done

debug: all
	./$(TARGET) ./roms/test.ch8

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)