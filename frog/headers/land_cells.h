#ifndef __LAND_CELLS_H__
#define __LAND_CELLS_H__

#include <actor_metaphor.h>
#include "extra_functions.h"

int cell_count; // total number of land cells
extern Role land_cell_role; // the role of a land cell actor

// a linked list measuring load
struct Load_List {
	int id; // the actor id of a land cell
	int load; // the load on that land cell
	struct Load_List *next;
};
typedef struct Load_List Load_List;

// the props of a land cell actor
typedef struct {
  int population_influx; // number of frogs to enter the cell that year
  int infection_level; // number of infected frogs to enter the cell that year
  int current_year; // the current year 
	Load_List *load_list; // the list of how loaded each actor is 
} Land_Cell;

// required for land cell role
void land_cell_initialisation(Actor *actor, void *props);
void land_cell_script(Actor *actor);
void land_cell_encore(Actor *actor);

// functions related to the creation and updating of load lists
Load_List* make_new_load_list(int id, int load);
void generate_load_list(Actor *actor);
void set_land_cell_load(Actor *actor, int id, int load);
void step_load(Actor *actor, int mod);
void message_load_to_all(Actor *actor);

// finds the least loaded land cell on the load list
int find_least_loaded_land_cell(Load_List *load_list);

#endif

