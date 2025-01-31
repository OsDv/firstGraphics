# Compiler settings
CC = gcc
#FRAMEWORKS = -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL

#PATHS
RAYLIB_PATH = ../raylib
INCLUDE_PATH = $(RAYLIB_PATH)/src
LIB_PATH = $(RAYLIB_PATH)/src

#source files
SRC = main.c
TARGET = app

# Build flags
CFLAGS = -Wall -Wextra -I$(INCLUDE_PATH)
LDFLAGS = $(LIB_PATH)/libraylib.a -lGL -lm  -lpthread -ldl -lrt

#default target

all: $(TARGET)

#Linking rules
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $<  $(LDFLAGS)  -o $@

#clean rule
clean:
	rm -f $(TARGET)
run:
	./$(TARGET) 2> /dev/null

.PHONY: all clean 
