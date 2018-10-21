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
LDLIBS= `pkg-config --libs sdl` -lm -lSDL_image

SRC = main.c image.c segmentation.c preprocessing.c
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}
PGR = ${SRC:.c=}
XOR = xorgate/xor.c xorgate/xorfun.c xorgate/xorgate.c
XOROBJ = ${XOR:.c=.o}
XORDEP = ${XOR:.c=.d}

all: main xorgate/xor

main: ${OBJ}
main.o: image.h preprocessing.h segmentation.h

xorgate/xor: ${XOROBJ}
xorgate/xor.o: xorgate/xorfun.h xorgate/xorgate.h

segmentation: image.o segmentation.o
segmentation.o: image.h

preprocessing: image.o preprocessing.o
preprocessing.o: image.h

-include ${DEP}
-include ${XORDEP}

.PHONY: clean
clean:
	# remove object files
	${RM} ${OBJ} ${XOROBJ}
	# remove dependency files
	${RM} ${DEP} ${XORDEP}
	# remove program file
	${RM} ${PGR} xorgate/xor

# END
