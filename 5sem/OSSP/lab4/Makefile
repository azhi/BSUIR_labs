ifeq ($(OS), Windows_NT)
  CC=g++
else
  CC=i586-mingw32msvc-g++
endif
CFLAGS=-c -O2 -mtune=native -D_UNICODE -DUNICODE -D_WIN32_WINNT=0x0501 -D_WIN32_IE=0x0700
LDFLAGS=-static -mwindows -lcomctl32
LIBCFLAGS=-DBUILDING_DLL
LIBFLAGS=-shared -Wl,-soname,

WNDRES=i586-mingw32msvc-windres

DB_SOURCE=DB.cpp
DB_OBJECT=$(DB_SOURCE:.cpp=.o)
DB_LIB=DB.dll

SHARED_SOURCES=abonent.cpp
CPP_SOURCES=main.cpp app.cpp db_wrapper.cpp $(SHARED_SOURCES)
OBJECTS=$(CPP_SOURCES:.cpp=.o)

RSRC=resources.rc
RS_OBJECTS=$(RSRC:.rc=.o)

EXECUTABLE=lab4.exe

$(EXECUTABLE): $(OBJECTS) $(RS_OBJECTS)
	$(CC) $(OBJECTS) $(RS_OBJECTS) -o $@ $(LDFLAGS)

all: $(EXECUTABLE) $(DB_LIB)

rebuild: clean all

$(DB_LIB): $(DB_OBJECT)
	$(CC) $(LIBFLAGS) $(DB_OBJECT) $(SHARED_SOURCES:.cpp=.o) -o $(DB_LIB)

$(DB_OBJECT): $(DB_SOURCE)
	$(CC) $(CFLAGS) $(LIBCFLAGS) $(DB_SOURCE) -o $(DB_OBJECT)

$(OBJECTS):
	$(CC) $(CFLAGS) $(@:.o=.cpp) -o $@

$(RS_OBJECTS):
	$(WNDRES) -i $(@:.o=.rc) -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(DB_OBJECT) $(DB_LIB)
