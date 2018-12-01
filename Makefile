# Makefile OCR #
# source of help : https://slashvar.github.io/2017/02/13/using-gnu-make.html
TARGET = ocr

# Compiler and Linker
CC = gcc -fsanitize=address
# Main compilation options
CFLAGS = -Wall -Wextra -Werror -std=c99 -O3
# Linker options
LDFLAGS = -pthread -export-dynamic

SDL = `pkg-config --cflags --libs sdl` -lSDL_image
GTK = `pkg-config --cflags --libs gtk+-3.0`
GSPELL = `pkg-config --cflags --libs gspell-1`
LM = -lm

SRC = $(shell find src -type f -name "*.c")

all: $(TARGET) tmpdir 

ocr: $(SRC)
	$(CC) -o $@ $(SRC) $(CFLAGS) $(LDFLAGS) $(SDL) $(GTK) $(GSPELL) $(LM)

tmpdir:
	mkdir -p tmp

.PHONY: clean
clean:
	# remove program file
	${RM} $(TARGET)
	# remove temp folder
	rm -r tmp
# END
