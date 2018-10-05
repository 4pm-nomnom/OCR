# Makefile OCR #
# source of help : https://slashvar.github.io/2017/02/13/using-gnu-make.html

# Options for pre-processor
CPPFLAGS = -MMD
# Setting the compiler and the default linker program
CC = gcc
# Main compilation options
CFLAGS = -Wall -Wextra -Werror -std=c99 #-O3
# Linker options
LDFLAGS =
# libs and path for linker
LDLIBS = `pkg-config --libs sdl` -lSDL image 

SRC = ocr.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: ocr

ocr: ${OBJ} 

.PHONY: clean
clean:
	${RM} ${OBJ}       # remove object files
	${RM} ${DEP}       # remove dependency files
	${RM} ocr          # remove main program

-include ${DEP}

 
# END
