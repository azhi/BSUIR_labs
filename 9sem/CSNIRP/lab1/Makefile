CC=x86_64-w64-mingw32-gcc
CFLAGS=-c -std=c11
LDFLAGS=
C_SOURCES=$(wildcard *.c)
OBJECTS=$(C_SOURCES:.c=.o)
PASSWORD_EXECUTABLE=password_crypt.exe
UK_GEN_EXECUTABLE=uk_gen_crypt.exe
HMAC_CHECK_EXECUTABLE=hmac_check.exe

all: $(C_SOURCES) $(PASSWORD_EXECUTABLE) $(UK_GEN_EXECUTABLE) $(HMAC_CHECK_EXECUTABLE)

$(PASSWORD_EXECUTABLE): crypt.o password_crypt.o
	$(CC) crypt.o password_crypt.o -o $@ $(LDFLAGS)

$(UK_GEN_EXECUTABLE): crypt.o gen_key_crypt.o
	$(CC) crypt.o gen_key_crypt.o -o $@ $(LDFLAGS)

$(HMAC_CHECK_EXECUTABLE): crypt.o hmac_check.o
	$(CC) crypt.o hmac_check.o -o $@ $(LDFLAGS)

%.c.:
	$(CC) $(CFLAGS) $< -o $@

password_test: $(PASSWORD_EXECUTABLE)
	/bin/bash password_test.sh

uk_gen_test: $(UK_GEN_EXECUTABLE)
	/bin/bash uk_gen_test.sh

hmac_test: $(HMAC_CHECK_EXECUTABLE)
	/bin/bash hmac_test.sh

test: password_test uk_gen_test hmac_test

clean:
	rm -f $(OBJECTS) $(PASSWORD_EXECUTABLE)
