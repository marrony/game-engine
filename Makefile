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

.SUFFIXES:
#.SUFFIXES: .cpp .o

all: prog$(EXT)

OBJ = main.o app.o app_win32.o app_x11.o

$(OBJ): %.o: %.cpp
	@echo '   ' CC $@; $(CC) -c $< -o $@

prog$(EXT): $(OBJ)
	@echo '   ' CC $@; $(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf prog$(EXT) *.o
