CC=gcc
#CFLAGS=-Wall -std=c99 -O0 -g -pg
CFLAGS=-Wall -std=c99 -O2 -fomit-frame-pointer -pipe -mtune=native
TARGETS=enc dec

rebuild: clean all

all: $(TARGETS)

$(TARGETS):
	$(CC) $(CFLAGS) $@.c -o $@

clean:
	rm -f *.o $(TARGETS)
