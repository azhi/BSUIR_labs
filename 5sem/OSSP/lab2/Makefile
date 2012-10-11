CC=i586-mingw32msvc-g++
CFLAGS=-c
LDFLAGS=-mwindows
CPP_SOURCES=main.cpp thread_pool.cpp worker.cpp console_io.cpp
OBJECTS=$(CPP_SOURCES:.cpp=.o)
EXECUTABLE=lab2.exe

all: $(CPP_SOURCES) $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 

$(OBJECTS): 
	$(CC) $(CFLAGS) $(@:.o=.cpp) -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
