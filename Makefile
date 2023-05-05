CC=g++
LDFLAGS=
ifeq ($(BUILD),debug)
CFLAGS +=-O0 -g
else
CFLAGS=-O0 -s -DNDEBUG
endif

ifeq ($(WSL_DISTRO_NAME),"")
WSL=
LDFLAGS +=-lpsapi
else
WSL=-D_WSL
endif

all: spotifytrack clean

debug:
	make "BUILD=debug"

spotifytrack: main.o
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.cpp
	$(CC) -c $(CFLAGS) $(WSL) $<

.PHONY: clean cleanest

clean:
	rm *.o

cleanest: clean
	rm spotifytrack
