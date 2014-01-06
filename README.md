darwins-dream-cpp
=================

An attempt to create digital species derived from simple natural selection rules.



RULE_SEEDS_Z0 :
The Z=0 plan is underground and reserved for new seeds (first cell of new creatures).
From there the creature will grow a cell at Z=1 and never come back at Z=0.
This is actually a performance improvement that greatly simplify the search for an available tile underground.


TODO:

- If possible show its lineage, at least generation in summary.

- Multi threading

- incorporate some kind of gravity constraint to have creatures a bit less linear? Maybe allow to grow from any existing cell, not only the head?
