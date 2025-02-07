# Detect the operating system
ifeq ($(OS),Windows_NT)
    # Windows
    SYSTEM = Windows
    TARGET = app.exe
    LDFLAGS = $(LIB_PATH)/libraylib.a -lopengl32 -lgdi32 -lwinmm
    CLEAN_CMD = $(TARGET)
    RM = del
else
    # Linux (or other Unix-like systems)
    SYSTEM = Linux
    TARGET = app
    LDFLAGS = $(LIB_PATH)/linux_libraylib.a -lGL -lm -lpthread -ldl -lrt
    CLEAN_CMD = rm -f $(TARGET)
endif

# Compiler settings
CC = gcc

# PATHS
RAYLIB_PATH = ../raylib
INCLUDE_PATH = $(RAYLIB_PATH)
LIB_PATH = $(RAYLIB_PATH)

# Source files
SRC = main.c 

# Build flags
CFLAGS = -Wall -Wextra -I$(INCLUDE_PATH) -std=c99 -g

# Default target
all: $(TARGET)

# Linking rules
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

# Clean rule
clean:
	$(RM) $(CLEAN_CMD)

run:
	./$(TARGET)

.PHONY: all clean run
