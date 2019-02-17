CC=g++
CFLAGS=-Wall -std=c++0x -pthread -pedantic -I/usr/include/GL -Ofast
LDFLAGS=-lGL -lGLU -lglut -lpthread
SOURCES=main.cpp utils.cpp viewer.cpp rendering.cpp Cell.cpp Creature.cpp Species.cpp CellsRegistry.cpp World.cpp Clocks.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC)  $(OBJECTS) -o $@  $(LDFLAGS)

.cpp.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean: 
	rm -f $(EXECUTABLE) *.o *~
