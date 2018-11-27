# Makefile OCR #
# source of help : https://slashvar.github.io/2017/02/13/using-gnu-make.html
TARGET = testgui testnn testnormalization

# Compiler and Linker
CC = gcc
# Main compilation options
CFLAGS = -Wall -Wextra -Werror -std=c99 -O3
# Linker options
LDFLAGS = -pthread -export-dynamic

SDLLIB = `pkg-config --cflags --libs sdl` -lSDL_image
GTKLIB = `pkg-config --cflags --libs gtk+-3.0`
MATHLIB = -lm

SRC_GUI = $(shell find gui -type f -name "*.c")

SRC_NN = $(shell find xorgate -type f -name "*.c") # change the src directory here !

SRC_NORMALIZATION =  $(shell find normalization -type f -name "*.c")

all: $(TARGET)

testgui: $(SRC_GUI)
	$(CC) -o $@ $(SRC_GUI) $(CFLAGS) $(LDFLAGS) $(SDLLIB) $(GTKLIB) $(MATHLIB)

testnn: $(SRC_NN)
	$(CC) -o $@ $(SRC_NN) $(CFLAGS) $(LDFLAGS) $(SDLLIB) $(GTKLIB) $(MATHLIB)

testnormalization: $(SRC_NORMALIZATION)
	$(CC) -o $@ $(SRC_NORMALIZATION) $(CFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	# remove program file
	${RM} $(TARGET)

# END
