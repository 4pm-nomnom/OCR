# Makefile OCR #
# source of help : https://slashvar.github.io/2017/02/13/using-gnu-make.html

# Options for pre-processor
CPPFLAGS= `pkg-config --cflags sdl` -MMD
# Setting the compiler and the default linker program
CC = gcc
# Main compilation options
CFLAGS = -Wall -Wextra -Werror -std=c99 -O3
# Linker options
LDFLAGS =
# libs and path for linker
LDLIBS= `pkg-config --libs sdl` -lSDL_image

SRC = ocr.c image.c segmentation.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
PGR = ${SRC:.c=}

all: ocr

ocr: ${OBJ}

segmentation: image.o segmentation.o
segmentation.o: image.h

-include ${DEP}

.PHONY: clean
clean:
	${RM} ${OBJ}       # remove object files
	${RM} ${DEP}       # remove dependency files
	${RM} ${PGR}       # remove program files


 
# END
