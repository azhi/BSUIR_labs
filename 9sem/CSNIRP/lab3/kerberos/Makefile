CC=x86_64-w64-mingw32-gcc
CFLAGS=-std=c11
LDFLAGS=-lws2_32
C_SOURCES=$(wildcard *.c)
OBJECTS=$(C_SOURCES:.c=.o)
AS_EXECUTABLE=auth_server.exe
TGS_EXECUTABLE=tg_server.exe
RS_EXECUTABLE=r_server.exe
CLIENT_EXECUTABLE=client.exe

all: $(C_SOURCES) $(AS_EXECUTABLE) $(TGS_EXECUTABLE) $(RS_EXECUTABLE) $(CLIENT_EXECUTABLE)

$(AS_EXECUTABLE): crypt.o auth_server.o
	$(CC) crypt.o auth_server.o -o $@ $(LDFLAGS)

$(TGS_EXECUTABLE): crypt.o ticket_granting_server.o
	$(CC) crypt.o ticket_granting_server.o -o $@ $(LDFLAGS)

$(RS_EXECUTABLE): crypt.o resource_server.o
	$(CC) crypt.o resource_server.o -o $@ $(LDFLAGS)

$(CLIENT_EXECUTABLE): crypt.o client.o
	$(CC) crypt.o client.o -o $@ $(LDFLAGS)

%.c.:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(AS_EXECUTABLE) $(TGS_EXECUTABLE) $(CLIENT_EXECUTABLE)
