CC=x86_64-w64-mingw32-gcc
CFLAGS=-std=c11
LDFLAGS=-lws2_32
C_SOURCES=$(wildcard *.c)
OBJECTS=$(C_SOURCES:.c=.o)
AS_EXECUTABLE=auth_server.exe
# TGS_EXECUTABLE=tgs_server.exe
# R_EXECUTABLE=r_server.exe
CLIENT_EXECUTABLE=client.exe

all: $(C_SOURCES) $(AS_EXECUTABLE) $(CLIENT_EXECUTABLE)

$(AS_EXECUTABLE): crypt.o auth_server.o
	$(CC) crypt.o auth_server.o -o $@ $(LDFLAGS)

$(CLIENT_EXECUTABLE): crypt.o client.o
	$(CC) crypt.o client.o -o $@ $(LDFLAGS)

%.c.:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(AS_EXECUTABLE) $(CLIENT_EXECUTABLE)
