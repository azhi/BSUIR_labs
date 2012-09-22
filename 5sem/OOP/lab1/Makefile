CC=g++
CFLAGS=-c -std=c++0x
LIBCFLAGS=-fPIC
LIBFLAGS=-shared -Wl,-soname,
LDFLAGS=-lSDL -lSDL_gfx -lboost_serialization
CPP_SOURCES=main.cpp main_controller.cpp sdl_controller.cpp scene.cpp
LIB_SOURCES=ellipse.cpp line.cpp rectangle.cpp star.cpp
OBJECTS=$(CPP_SOURCES:.cpp=.o)
LIB_OBJECTS=$(LIB_SOURCES:.cpp=.o)
LIB_NAMES=$(LIB_OBJECTS:.o=)
LIBS=$(foreach lib_name, $(LIB_NAMES), lib$(lib_name).so)
EXECUTABLE=Painter

all: $(LIBS) $(CPP_SOURCES) $(addprefix models/, $(LIB_SOURCES)) $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 

$(OBJECTS): 
	$(CC) $(CFLAGS) $(@:.o=.cpp) -o $@

$(addprefix models/, $(LIB_OBJECTS)):
	$(CC) $(LIBCFLAGS) $(CFLAGS) $(@:.o=.cpp) -o $@

define BUILD_LIB 
	$(CC) $(LIBFLAGS)lib$(1).so -o lib$(1).so models/$(1).o $(LDFLAGS)

endef	

$(LIBS): $(addprefix models/, $(LIB_OBJECTS))
	$(foreach lib_name, $(LIB_NAMES), $(call BUILD_LIB,$(lib_name)))

clean:
	rm -f $(OBJECTS) $(addprefix models/, $(LIB_OBJECTS)) $(EXECUTABLE) $(LIBS)
