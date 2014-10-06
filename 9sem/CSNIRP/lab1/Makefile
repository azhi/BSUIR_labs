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
	rm test_out.txt test_diff.txt
	wine $(EXECUTABLE) --encode 12345678 test.txt test_out.txt
	wine $(EXECUTABLE) --decode 12345678 test_out.txt test_diff.txt
	cmp test{,_diff}.txt && echo 'OK'

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
