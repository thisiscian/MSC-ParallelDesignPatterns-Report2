#ifndef __LAND_CELLS_H__
#define __LAND_CELLS_H__

#include <actor_metaphor.h>
#include "extra_functions.h"

int cell_count;
extern Role land_cell_role;

struct Load_List {
	int id;
	int load;
	struct Load_List *next;
};

typedef struct Load_List Load_List;

typedef struct {
	long state;
  int population_influx;
  int infection_level;
	Load_List *load_list;
} Land_Cell;

void land_cell_initialisation(Actor *actor);
void land_cell_script(Actor *actor);
void generate_load_list(Actor *actor);
void set_land_cell_load(Actor *actor, int id, int load);
void step_load(Actor *actor, int mod);
void message_load_to_all(Actor *actor);
Load_List* make_new_load_list(int id, int load);
int find_least_loaded_process(Load_List *load_list);

#endif

