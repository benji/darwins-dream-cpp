Darwin's dream (C++ version)
=================

<b>Abstract</b><br/>

The intention is to demonstrate how the constraints for survival drives the shape of the species through natural selection.

The program generates some initial species with random DNA, and creates a few creatures.<br/>
Each creature is composed of several Cells.<br/>
The DNA dictates the growth direction for each Cell.<br/>
The program will launch a window that will display a 3D view of the World on the left, and the shape of the dominant species on the right, rotating.

![](https://raw.github.com/benji/darwins-dream-cpp/master/docs/screenshot-4.png)

<b>Usage</b><br/>

Commands:
  * F - Fullscreen
  * S - Summary: Prints a summary of the current species (with some lineage info)
  * [SPACE] - pause/resume simulation
  * N - Next: when paused, runs a single cycle
  * F1 - perform a consitency check on the cells (debug)
  * '=' - Switches the right view to show either the biggest creature or the typical DNA creature from the current dominant species.
  * [ESC]/Q - exit

Please see common.h for settings/customizations/performance.

<b>Constraints</b><br/>
There are strict rules for survival:
  * Sunlight constraint: the creature must gather enough energy from its cells to survive. The more cells, the more energy it needs. If a creature is in the shade of another, it will not get as much sunlight (see CONTRAINT_NEED_SUN, MIN_ENERGY_PER_CELL, SUNLIGHT_LOSS_RATIO)
  * Balancing constraint: the creatures must not lean too much in one direction (see CONSTRAINT_BALANCING, BALANCING_THRESHOLD)
  * Other constraints exist about the age of a creature and its ability to grow (see MAX_CREATURE_AGE, GROW_OR_DIE)

<b>Reproduction</b><br/>
(see REPRODUCTION_RATE)<br/>
A recent addition has been to make a new creature appear close to its parent (see LOCALITY_ENABLED, LOCALITY_RADIUS). This creates patches of colors corresponding to competing territories of species.

<b>Mutations</b><br/>
(see MUTATION_RATE)<br/>
One of the DNA segment that dictates the direction of growth of a cell will be altered. The resulting species may or may not be fit for survival.

<b>Spontaneous species</b><br/>
(see SPONANEOUS_SPECIES_PER_CYCLE)<br/>
Randomly sprinkle new species out of nowhere to improve competition.

<b>Performance</b><br/>
You can configure the number of threads to use in common.h. I recommend to use NbCores - 1 (see NB_THREADS).

<b>TODO</b><br/>
- Allow to grow from any existing cell, not only from the head (see PROGRESSIVE_DNA)

<b>Technicalities</b><br/>

RULE_SEEDS_Z0:<br/>
The Z=0 plan is underground and reserved for new seeds (first cell of new creatures).<br/>
From there the creature will grow a cell at Z=1 and never come back at Z=0.<br/>
This is actually a performance improvement that greatly simplify the search for an available tile underground.
