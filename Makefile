# ==========================================
# Compiler and Flags Configuration
# ==========================================
CC       := clang
CFLAGS   := -Wall -Wextra -std=c11 -O2 $(shell sdl2-config --cflags)
LDFLAGS  := $(shell sdl2-config --libs) -lm

# ==========================================
# Directory Layout
# ==========================================
# If your files are in the root directory, leave these as .
# If you use subfolders, update them here (e.g., SRC_DIR := src)
SRC_DIR  := src lib
INC_DIR  := include
OBJ_DIR  := obj
BIN_DIR  := bin

TARGET   := $(BIN_DIR)/chip8
CPPFLAGS := -I$(INC_DIR)

# ==========================================
# Source and Object File Detection
# ==========================================
SRCS := $(foreach dir, $(SRC_DIR), $(wildcard $(dir)/*.c))
OBJS := $(addprefix $(OBJ_DIR)/, $(notdir $(SRCS:.c=.o)))

# ==========================================
# Build Rules
# ==========================================
.PHONY: all clean run debug

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Compile source files into object files
$(OBJ_DIR)/%.o: src/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: lib/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

# Create directories if they don't exist
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Run the emulator
run: all
	./$(TARGET)

debug: all
	./$(TARGET) ./roms/test.ch8

# Clean build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)