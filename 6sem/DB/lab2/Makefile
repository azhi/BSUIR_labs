CC=clang++
#CFLAGS=-c -Wall -std=c++11 -O0 -ggdb
CFLAGS=-c -Wall -std=c++11 -O2 -fomit-frame-pointer -pipe -mtune=native
LDFLAGS=-Wl,--as-needed -Wl,-O1 -lboost_program_options -lboost_serialization
CPP_SOURCES=$(wildcard *.cpp)
OBJECTS=$(CPP_SOURCES:.cpp=.o)
OBJECTS_STR=$(OBJECTS:.o=.str.o)
OBJECTS_NUM=$(OBJECTS:.o=.num.o)
EXECUTABLE_STR=vsam_str
EXECUTABLE_NUM=vsam_num

rebuild: clean all

all: $(CPP_SOURCES) $(EXECUTABLE_STR) $(EXECUTABLE_NUM)

$(EXECUTABLE_STR): $(OBJECTS_STR)
	$(CC) $(OBJECTS_STR) $(LDFLAGS) -o $@

$(EXECUTABLE_NUM): $(OBJECTS_NUM)
	$(CC) $(OBJECTS_NUM) $(LDFLAGS) -o $@

$(OBJECTS_STR):
	$(CC) $(CFLAGS) -DTYPE=0 $(CPP_SOURCES) -o $(@)

$(OBJECTS_NUM):
	$(CC) $(CFLAGS) -DTYPE=1 $(CPP_SOURCES) -o $(@)

clean:
	rm -f $(OBJECTS:.o=.str.o) $(OBJECTS:.o=.num.o) $(EXECUTABLE_STR) $(EXECUTABLE_NUM)
