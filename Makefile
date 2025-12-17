# Puzzle Attack Makefile
# Cross-platform build system for Windows, macOS, and Linux

# Project configuration
PROJECT_NAME = puzzle-attack
VERSION = 0.1.0

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
ASSETS_DIR = assets

# Source files
CLIENT_SRC = $(wildcard $(SRC_DIR)/client/*.c)
SERVER_SRC = $(wildcard $(SRC_DIR)/server/*.c)
SHARED_SRC = $(wildcard $(SRC_DIR)/shared/*.c)
MAIN_SRC = $(SRC_DIR)/main.c

# Compiler
CC = cc

# Detect platform
UNAME_S := $(shell uname -s 2>/dev/null || echo Windows)

# Common flags
CFLAGS = -std=c99 -Wall -Wextra -I$(INCLUDE_DIR)
LDFLAGS =

# Debug/Release configuration
ifdef DEBUG
    CFLAGS += -g -O0 -DDEBUG
else
    CFLAGS += -O2 -DNDEBUG
endif

# Platform-specific configuration
ifeq ($(UNAME_S),Darwin)
    # macOS
    PLATFORM = macos
    RAYLIB_CFLAGS = $(shell pkg-config --cflags raylib)
    RAYLIB_LDFLAGS = $(shell pkg-config --libs raylib)
    LDFLAGS += -framework Cocoa -framework IOKit -framework CoreVideo
    TARGET = $(BUILD_DIR)/$(PROJECT_NAME)
else ifeq ($(UNAME_S),Linux)
    # Linux
    PLATFORM = linux
    RAYLIB_CFLAGS = $(shell pkg-config --cflags raylib)
    RAYLIB_LDFLAGS = $(shell pkg-config --libs raylib)
    LDFLAGS += -lm -lpthread -ldl
    TARGET = $(BUILD_DIR)/$(PROJECT_NAME)
else
    # Windows (MinGW)
    PLATFORM = windows
    # Adjust these paths for your Windows raylib installation
    RAYLIB_PATH ?= C:/raylib/raylib
    RAYLIB_CFLAGS = -I$(RAYLIB_PATH)/src
    RAYLIB_LDFLAGS = -L$(RAYLIB_PATH)/src -lraylib -lopengl32 -lgdi32 -lwinmm
    TARGET = $(BUILD_DIR)/$(PROJECT_NAME).exe
endif

# Combine all source files for client build
ALL_SRC = $(MAIN_SRC) $(CLIENT_SRC) $(SERVER_SRC) $(SHARED_SRC)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(ALL_SRC))

# Default target
.PHONY: all
all: $(TARGET)

# Create build directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_DIR)/client
	mkdir -p $(BUILD_DIR)/server
	mkdir -p $(BUILD_DIR)/shared

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(RAYLIB_CFLAGS) -c $< -o $@

# Link final executable
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(RAYLIB_LDFLAGS) $(LDFLAGS)

# Clean build artifacts
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)/*

# Run the game
.PHONY: run
run: $(TARGET)
	./$(TARGET)

# Debug build
.PHONY: debug
debug:
	$(MAKE) DEBUG=1 all

# Print configuration (useful for debugging build issues)
.PHONY: info
info:
	@echo "Platform: $(PLATFORM)"
	@echo "Compiler: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "RAYLIB_CFLAGS: $(RAYLIB_CFLAGS)"
	@echo "RAYLIB_LDFLAGS: $(RAYLIB_LDFLAGS)"
	@echo "LDFLAGS: $(LDFLAGS)"
	@echo "Sources: $(ALL_SRC)"
	@echo "Target: $(TARGET)"

# Help
.PHONY: help
help:
	@echo "Puzzle Attack Build System"
	@echo ""
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@echo "  all     - Build the game (default)"
	@echo "  run     - Build and run the game"
	@echo "  debug   - Build with debug symbols"
	@echo "  clean   - Remove build artifacts"
	@echo "  info    - Print build configuration"
	@echo "  help    - Show this help message"
