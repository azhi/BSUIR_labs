CC=x86_64-w64-mingw32-gcc
CFLAGS=-std=c11
LDFLAGS=-lws2_32
C_SOURCES=$(wildcard *.c)
OBJECTS=$(C_SOURCES:.c=.o)
SERVER_EXECUTABLE=server.exe
CLIENT_EXECUTABLE=client.exe

all: $(C_SOURCES) $(SERVER_EXECUTABLE) $(CLIENT_EXECUTABLE)

$(SERVER_EXECUTABLE): crypt.o server.o
	$(CC) crypt.o server.o -o $@ $(LDFLAGS)

$(CLIENT_EXECUTABLE): crypt.o client.o
	$(CC) crypt.o client.o -o $@ $(LDFLAGS)

%.c.:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(SERVER_EXECUTABLE) $(CLIENT_EXECUTABLE)
