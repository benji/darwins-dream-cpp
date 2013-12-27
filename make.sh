#!/bin/sh

g++ -Wall -std=c++0x -pthread -pedantic -o main main.cpp utils.cpp rendering.cpp Cell.cpp Creature.cpp Species.cpp CellsRegistry.cpp World.cpp Clocks.cpp -I/usr/include/GL  -lGL -lGLU -lglut
