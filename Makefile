UNAME_S := $(shell sh -c 'uname -s')

CXX = g++
CXX_QUIT = @echo '   ' CXX $@;
CXXFLAGS =

ifeq (MINGW,$(findstring MINGW,$(UNAME_S)))
EXT = .exe
LDFLAGS = -lopengl32 -lgdi32
else
EXT = 
LDFLAGS = -lGL -lX11
endif

ifeq (mingw,$(findstring mingw,$(CXX)))
EXT = .exe
LDFLAGS = -lopengl32 -lgdi32
endif

#LDFLAGS += -mwindows

.SUFFIXES:
#.SUFFIXES: .cpp .o

all: prog$(EXT)

OBJ = main.o app.o app_win32.o app_x11.o

$(OBJ): %.o: %.cpp app.h
	$(CXX_QUIT) $(CXX) $(CXXFLAGS) -c $< -o $@

prog$(EXT): $(OBJ)
	$(CXX_QUIT) $(CXX) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf prog$(EXT) *.o
