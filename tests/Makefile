# Makefile OCR #
# source of help : https://slashvar.github.io/2017/02/13/using-gnu-make.html
TARGET = test

# Compiler and Linker
CC = gcc
# Main compilation options
CFLAGS = -Wall -Wextra -Werror -std=c99 -O3
# Linker options
LDFLAGS = -pthread -export-dynamic

SDLLIB = `pkg-config --cflags --libs sdl` -lSDL_image
GTKLIB = `pkg-config --cflags --libs gtk+-3.0`
MATHLIB = -lm

SRC = $(shell find src -type f -name "*.c") # change the src directory here !

all: $(TARGET) 

ocr: $(SRC)
	$(CC) -o $@ $(SRC) $(CFLAGS) $(LDFLAGS) $(SDLLIB) $(GTKLIB) $(MATHLIB)

.PHONY: clean
clean:
	# remove program file
	${RM} $(TARGET)

# END
