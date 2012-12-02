CC=g++
CFLAGS=-c -ggdb -std=c++0x
LDFLAGS=-lSDL -lSDL_gfx
CPP_SOURCES=main.cpp main_controller.cpp sdl_controller.cpp scene.cpp tree.cpp grammatic_analyzer.cpp models/ellipse.cpp models/line.cpp models/rectangle.cpp
OBJECTS=$(CPP_SOURCES:.cpp=.o)
EXECUTABLE=Gramm

all: $(CPP_SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
