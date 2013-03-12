CC=g++
#CFLAGS=-c -Wall -std=c++11 -O0 -ggdb
CFLAGS=-c -Wall -std=c++11 -O2 -fomit-frame-pointer -pipe -mtune=native
LDFLAGS=-Wl,--as-needed -Wl,-O1 -lboost_program_options -lboost_serialization
CPP_SOURCES=$(wildcard *.cpp)
OBJECTS=$(CPP_SOURCES:.cpp=.o)
EXECUTABLE=hash

all: $(CPP_SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJECTS):
	$(CC) $(CFLAGS) $(@:.o=.cpp) -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

rebuild: clean all
