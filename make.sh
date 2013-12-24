#!/bin/sh

g++ -Wall -pedantic -o main main.cpp utils.cpp Cell.cpp Creature.cpp Species.cpp World.cpp -I/usr/include/GL  -lGL -lGLU -lglut
