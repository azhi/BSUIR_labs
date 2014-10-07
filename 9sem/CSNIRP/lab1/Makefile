CC=x86_64-w64-mingw32-gcc
CFLAGS=-c -std=c11
LDFLAGS=
C_SOURCES=$(wildcard *.c)
OBJECTS=$(C_SOURCES:.c=.o)
PASSWORD_EXECUTABLE=password_crypt.exe

all: $(C_SOURCES) $(PASSWORD_EXECUTABLE)

$(PASSWORD_EXECUTABLE): crypt.o password_crypt.o
	$(CC) crypt.o password_crypt.o -o $@ $(LDFLAGS)

%.c.:
	$(CC) $(CFLAGS) $< -o $@

password_test: $(PASSWORD_EXECUTABLE)
	/bin/bash password_test.sh

test: password_test

clean:
	rm -f $(OBJECTS) $(PASSWORD_EXECUTABLE)
