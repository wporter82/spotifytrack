CC=g++
LDFLAGS=
CFLAGS=-Wno-write-strings

all: spotifytrack clean

spotifytrack: main.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.cpp
	$(CC) -c $(CFLAGS) $<

.PHONY: clean cleanest

clean:
	rm *.o

cleanest: clean
	rm spotifytrack
