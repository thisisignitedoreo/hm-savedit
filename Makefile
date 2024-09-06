# Variables
TARGET = savedit
SRC = src/main.c
CC = gcc
CFLAGS = -w -std=c99 -pedantic

# Default target
all: linux windows

# Compile the target
linux: $(SRC)
	$(CC) $(CFLAGS) -I./ext/raylib/include -L./ext/raylib/lib -o $(TARGET) $^ -l:libraylib.a -lm -ldl -ggdb

windows: $(SRC)
	x86_64-w64-mingw32-$(CC) -mwindows $(CFLAGS) -I./ext/raylib-win/include -L./ext/raylib-win/lib -o $(TARGET).exe $^ -l:libraylib.a -lwinmm -lgdi32

windows-console: $(SRC)
	x86_64-w64-mingw32-$(CC) $(CFLAGS) -I./ext/raylib-win/include -L./ext/raylib-win/lib -o $(TARGET).exe $^ -l:libraylib.a -lwinmm -lgdi32 -ggdb

clean:
	rm -f $(TARGET) $(TARGET).exe

.PHONY: all clean linux windows windows-console

