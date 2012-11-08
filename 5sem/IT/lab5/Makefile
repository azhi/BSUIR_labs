CC=g++
CFLAGS=-c -std=c++0x
LDFLAGS=-lm
CPP_SOURCES=$(wildcard *.cpp)
OBJECTS=$(CPP_SOURCES:.cpp=.o)
EXECUTABLE=BigNums

all: $(CPP_SOURCES) $(OBJECTS) $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 

$(OBJECTS): 
	$(CC) $(CFLAGS) $(@:.o=.cpp) -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
