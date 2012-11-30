CC=g++
CFLAGS=-c -fPIC -std=c++0x -ggdb
CPP_SOURCES=$(wildcard *.cpp)
OBJECTS=$(CPP_SOURCES:.cpp=.o)
PREFIX=../../../bin/

all: $(OBJECTS) $(CPP_SOURCES)

$(OBJECTS): $(CPP_SOURCES)
	$(CC) $(CFLAGS) $(@:.o=.cpp) -o $(addprefix $(PREFIX), $@)

clean:
	rm -rf $(addprefix $(PREFIX), $(OBJECTS)
