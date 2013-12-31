CC=g++
CFLAGS=-Wall -std=c++0x -pthread -pedantic -I/usr/include/GL
LDFLAGS=-lGL -lGLU -lglut
SOURCES=main.cpp utils.cpp rendering.cpp Cell.cpp Creature.cpp Species.cpp CellsRegistry.cpp World.cpp Clocks.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC)  $(OBJECTS) -o $@  $(LDFLAGS)

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean: 
	rm -f $(EXECUTABLE) *.o *~
