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

main.o: main.cpp app.h
	$(CC) -c main.cpp -o main.o

app.o: app.cpp app.h
	$(CC) -c app.cpp -o app.o
	
app_x11.o: app_x11.cpp app.h
	$(CC) -c app_x11.cpp -o app_x11.o

app_win32.o: app_win32.cpp app.h
	$(CC) -c app_win32.cpp -o app_win32.o

prog$(EXT): main.o app.o app_x11.o app_win32.o
	$(CC) -o prog$(EXT) main.o app.o app_x11.o app_win32.o $(LDFLAGS)

clean:
	rm -rf prog$(EXT) *.o
