CC=g++
LDFLAGS=
CFLAGS=

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
