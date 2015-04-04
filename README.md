darwins-dream-cpp
=================
 
An attempt to create digital species derived from simple natural selection rules.


The program generates some initial species with random DNA, and creates a few creatures.
Each creature is composed of several Cells.
The DNA dictates the growth direction for each Cell.
The program will launch a window that will display a 3D view of the World on the left, and the shape of the dominant species on the right, rotating.

![](https://raw.github.com/benji/darwins-dream-cpp/master/docs/screenshot-3.png)

Please see common.h for settings/customizations/performance.

Commands:
  * F - Fullscreen
  * S - pring Summary of the current species (with some lineage info)
  * [SPACE] - pause/resume simulation
  * N - when paused, run a single cycle
  * F1 - perform a consitency check on the cells (debug)
  * '=' - Switches the right view to show either the biggest creature or the typical DNA creature from the current dominant species.
  * [ESC]/Q - exit

Constraints:
There are strict rules for survival:
  * Sunlight constraint: the creature must gather enough energy from its cells to survive. The more cells, the more energy it needs. If a creature is in the shade of another, it will not get as much sunlight (see CONTRAINT_NEED_SUN, SUNLIGHT_LOSS_RATIO)
  * Balancing constraint: the creatures must not lean too much in one direction (see CONSTRAINT_BALANCING, BALANCING_THRESHOLD)
  * Other constraint exist about the age of a creature and its ability to grow (see MAX_CREATURE_AGE, GROW_OR_DIE)

Reproduction:
A recent addition has been to make the children appear close to their parents (see LOCALITY_ENABLED, LOCALITY_RADIUS). This creates patches of colors corresponding to competing territories of species.
(see REPRODUCTION_RATE)

Mutation:
One of the DNA segment that dictates the direction of growth of a cell will be altered. The resulting species may or may not be fit for survival.
(see MUTATION_RATE)



Performance:
You can configure the number of threads to use in common.h. I recommend to use NbCores - 1 (see NB_THREADS).

RULE_SEEDS_Z0:
The Z=0 plan is underground and reserved for new seeds (first cell of new creatures).
From there the creature will grow a cell at Z=1 and never come back at Z=0.
This is actually a performance improvement that greatly simplify the search for an available tile underground.

TODO:
- Allow to grow from any existing cell, not only from the head (see PROGRESSIVE_DNA)
