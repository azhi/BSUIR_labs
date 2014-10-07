CC=x86_64-w64-mingw32-gcc
CFLAGS=-c -std=c11
LDFLAGS=
C_SOURCES=$(wildcard *.c)
OBJECTS=$(C_SOURCES:.c=.o)
EXECUTABLE=lab1.exe

all: $(C_SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJECTS): $(C_SOURCES)
	$(CC) $(CFLAGS) $< -o $@

test: $(EXECUTABLE)
	/bin/bash test.sh

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
