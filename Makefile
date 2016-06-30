CC=g++
LDFLAGS=
ifeq ($(BUILD),debug)
CFLAGS +=-Wno-write-strings -O0 -g
else
CFLAGS=-Wno-write-strings -O3 -s -DNDEBUG
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
