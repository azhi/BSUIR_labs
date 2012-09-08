CC=g++
CFLAGS=-c -std=c++0x
LDFLAGS=-lSDL -lSDL_gfx
CPP_SOURCES=main.cpp main_controller.cpp sdl_controller.cpp scene.cpp models/ellipse.cpp models/line.cpp models/rectangle.cpp
# H_SOURCES=models/figure.h models/line.h models/rectangle.h models/ellipse.h
OBJECTS=$(CPP_SOURCES:.cpp=.o)
EXECUTABLE=Painter

all: $(CPP_SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
