CC=g++
LIBFLAGS=-shared -fPIC -Wl,-soname,libgrom.so
LDFLAGS=-lexpat
OBJECTS=$(wildcard bin/*.o)
LIB=libgrom.so

all: $(LIB) system xml_utils

install: all
	cp build/libgrom.so /usr/lib/libgrom.so

system:
	$(MAKE) -C source/Grom/System

xml_utils:
	$(MAKE) -C source/Grom/XmlUtils

$(LIB): system xml_utils
	$(CC) $(LIBFLAGS) $(OBJECTS) -o build/$(LIB) $(LDFLAGS)

clean:
	rm -f build/libgrom.so bin/*.o
