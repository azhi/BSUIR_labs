CC=g++
#CFLAGS=-c -Wall -std=c++0x -O0 -ggdb
CFLAGS=-c -Wall -std=c++0x -O2 -fomit-frame-pointer -pipe -mtune=native
LDFLAGS=-Wl,--as-needed -Wl,-O1
CPP_SOURCES=$(wildcard *.cpp)
OBJECTS=$(CPP_SOURCES:.cpp=.o)
EXECUTABLE=hash

all: $(CPP_SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJECTS):
	$(CC) $(CFLAGS) $(@:.o=.cpp) -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

rebuild: clean all
