#ifndef __LAND_CELLS_H__
#define __LAND_CELLS_H__

#include <actormetaphor.h>
#include "extra_functions.h"

int initial_cell_count;
int land_cell_id_counter;
Role land_cell_role;

typedef struct {
  long *state;
  int land_cell_id;
  int population_influx;
  int infection_level;
} Land_Cell;

void land_cell_initialisation(Actor* actor);
void land_cell_script(Actor* actor);
void land_cell_reaction(Actor* actor, int message_type, int next_message_size);

#endif

