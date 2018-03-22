CC=g++
LDFLAGS=-lpsapi
ifeq ($(BUILD),debug)
CFLAGS +=-O0 -g
else
CFLAGS=-O0 -s -DNDEBUG
endif

all: spotifytrack clean

debug:
	make "BUILD=debug"

spotifytrack: main.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.cpp
	$(CC) -c $(CFLAGS) $<

.PHONY: clean cleanest

clean:
	rm *.o

cleanest: clean
	rm spotifytrack
