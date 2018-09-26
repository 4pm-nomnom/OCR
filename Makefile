# Makefile OCR #
# source of help : https://slashvar.github.io/2017/02/13/using-gnu-make.html


# Setting the compiler and the default linker program
CC = gcc
# Options for pre-processor
CPPFLAGS = 
# Main compilation options
CFLAGS = -Wall -Wextra -std=c99 -O3
# Linker options
LDFLAGS =
# libs and path for linker
LDLIBS =

# main: main.o hey.o fun.o
ocr: ocr.o

# clean options
clean:
	${RM} *.o       # remove object files
	${RM} ocr       # remove main program


# END
