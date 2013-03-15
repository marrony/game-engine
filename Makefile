UNAME_S := $(shell sh -c 'uname -s')

CC = g++

ifeq (MINGW,$(findstring MINGW,$(UNAME_S)))
EXT = .exe
LDFLAGS = -lopengl32 -lgdi32
else
EXT = 
LDFLAGS = -lGL -lX11
endif

ifeq (mingw,$(findstring mingw,$(CC)))
EXT = .exe
LDFLAGS = -lopengl32 -lgdi32
endif

all: prog$(EXT)

main.o: main.cpp
	$(CC) -c main.cpp -o main.o

prog$(EXT): main.o
	$(CC) -o prog$(EXT) main.o $(LDFLAGS)

clean:
	rm -rf prog$(EXT) *.o
