CC=g++
CFLAGS=-c -std=c++0x
INCLUDE_PATH=-I./grom/source/
LIB_PATH=-L./grom/
LDFLAGS=-lpthread -lexpat -lgrom
CPP_SOURCES=$(wildcard *.cpp)
OBJECTS=$(CPP_SOURCES:.cpp=.o)
EXECUTABLE=lab2

all: $(CPP_SOURCES) $(OBJECTS) $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LIB_PATH) -o $@ $(LDFLAGS) 

$(OBJECTS): 
	$(CC) $(CFLAGS) $(INCLUDE_PATH) $(@:.o=.cpp) -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
