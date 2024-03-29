.PHONY = clean prepare
CC=gcc
CFILES=$(wildcard src/*.c)

all: clean prepare autoanswer

autoanswer: build_o
	$(CC) *.o -o bin/autoanswer -g `pkg-config --cflags --libs libpjproject`
	rm *.o

build_o: $(CFILES)
	$(CC) $(CFILES) -c -I includes -g

clean:
	rm -rf bin

prepare:
	mkdir bin
	cp sounds/*.wav bin/
