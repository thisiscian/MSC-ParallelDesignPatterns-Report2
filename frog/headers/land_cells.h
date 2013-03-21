#ifndef __LAND_CELLS_H__
#define __LAND_CELLS_H__

#include <actor_metaphor.h>
#include "extra_functions.h"

int initial_cell_count;
extern Role land_cell_role;

typedef struct {
	long state;
  int population_influx;
  int infection_level;
} Land_Cell;

void land_cell_initialisation(Actor* actor);
void land_cell_script(Actor* actor);

#endif

