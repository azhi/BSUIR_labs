CC=i586-mingw32msvc-g++
CFLAGS=-c -D_UNICODE -DUNICODE
LDFLAGS=-static -mwindows -lcomctl32
LIBCFLAGS=-DBUILDING_DLL
LIBFLAGS=-shared -Wl,-soname,

DB_SOURCE=DB.cpp
DB_OBJECT=$(DB_SOURCE:.cpp=.o)
DB_LIB=DB.dll

SHARED_SOURCES=abonent.cpp
CPP_SOURCES=main.cpp app.cpp db_wrapper.cpp $(SHARED_SOURCES)
OBJECTS=$(CPP_SOURCES:.cpp=.o)

EXECUTABLE=lab4.exe

$(EXECUTABLE): $(OBJECTS) $(DB_LIB) $(RESOURCES)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

all: $(CPP_SOURCES) $(EXECUTABLE) $(DB_LIB)

rebuild: clean all

$(DB_LIB): $(DB_OBJECT)
	$(CC) $(LIBFLAGS) $(DB_OBJECT) $(SHARED_SOURCES:.cpp=.o) -o $(DB_LIB)

$(DB_OBJECT): $(DB_SOURCE)
	$(CC) $(CFLAGS) $(LIBCFLAGS) $(DB_SOURCE) -o $(DB_OBJECT)

$(OBJECTS):
	$(CC) $(CFLAGS) $(@:.o=.cpp) -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(DB_OBJECT) $(DB_LIB)
