darwins-dream-cpp
=================

An attempt to create digital species derived from simple natural selection rules.


The program generates some initial species with random DNA, and creates a few creatures.
Each creature is composed of several Cells.
The DNA dictates the growth direction for each Cell.
The program will launch a window that will display a 3D view of the World on the left, and the shape of the dominant species on the right, rotating.

Please see common.h for settings/customizations/performance.

Commands:
  * F - Fullscreen
  * S - pring Summary of the current species
  * <SPACE> - pause/resume simulation
  * N - when paused, run a single cycle
  * F1 - perform a consitency check on the cells (debug)
  * <ESC> - exit

RULE_SEEDS_Z0 :
The Z=0 plan is underground and reserved for new seeds (first cell of new creatures).
From there the creature will grow a cell at Z=1 and never come back at Z=0.
This is actually a performance improvement that greatly simplify the search for an available tile underground.


TODO:

- If possible show its lineage, at least generation in summary.

- Multi threading for Local reproduction, benchmark

- a bit less linear? Maybe allow to grow from any existing cell, not only the head?

One way would be to have a DNA model being vector<int[2]> containing for every strand:
- the position of the cell to grow
- the direction of growth


- Ideas of constraints to add:
  * some kind of gravity to have creatures a bit more balanced
  * for fun we could try to kill creatures whose head touches another creature, and see what shapes derive from it?

- Think about the benefits of using a more subtle way than to kill the creature when it doesn't fit the constraint.
  Maybe it can just be a slower growth or a limited growth.
  For example it's a problem with the gravity constraint because none of the species randomly generated will respect that so all the species will die right away.
  There should be allowed to grow at least up to the point where they would fall and just stay like that until a mutation makes their DNA better fit for the contraint.

Idea:
we start with 1 Species with 1 DNA (cell go up).
A mutation will either change an existing DNA strand (but never the first) OR add a new DNA strand to the species.
After every mutation, a check should be performed to verify the constraint of BALANCING and to check that the structure doesn't conflict with itself.
-> faster because the gravity constraint will onl be checked on the creation of a new species, instead of at every growth.


REFERENCES:

- mutex: http://www.cplusplus.com/reference/mutex/mutex/














