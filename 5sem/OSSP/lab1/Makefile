CC=i586-mingw32msvc-g++
CFLAGS=-c
RESCC=i586-mingw32msvc-windres
LDFLAGS=-mwindows
CPP_SOURCES=$(wildcard *.cpp)
OBJECTS=$(CPP_SOURCES:.cpp=.o)
RESOURCE=resource.rc
RESOURCEOBJECT=resource.o
EXECUTABLE=lab1.exe

all: $(CPP_SOURCES) $(RESOURCE) $(EXECUTABLE)

rebuild: clean all

$(EXECUTABLE): $(OBJECTS) $(RESOURCEOBJECT)
	$(CC) $(OBJECTS) $(RESOURCEOBJECT) -o $@ $(LDFLAGS)

$(OBJECTS):
	$(CC) $(CFLAGS) $(@:.o=.cpp) -o $@

$(RESOURCEOBJECT):
	$(RESCC) -i $(RESOURCE) -o $(RESOURCEOBJECT)

clean:
	rm -f $(OBJECTS) $(RESOURCEOBJECT) $(EXECUTABLE)
