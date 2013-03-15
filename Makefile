all: prog

UNAME_S := $(shell sh -c 'uname -s')

CC = g++

ifeq (MINGW,$(findstring MINGW,$(UNAME_S)))
LDFLAGS = -lopengl32 -lgdi32
else
LDFLAGS = -lGL -lX11
endif

main.o: main.cpp
	$(CC) -c main.cpp -o main.o

prog: main.o
	$(CC) -o prog main.o $(LDFLAGS)

