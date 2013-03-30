#ifndef __FROGS_H__
#define __FROGS_H__

#include <actor_metaphor.h>
#include "extra_functions.h"
#include "../provided-functions/ran2.h"
#include "../provided-functions/frog-functions.h"

int initial_frog_count; // the number of frogs at the start of year 0
int initial_diseased_frog_count; // the number of diseased frogs at the start of year 0
int max_frog_count; // the total number of frogs before the program exits
extern Role frog_role; // the role of a frog actor


// The props of a frog actor
typedef struct {
	long state; // used for random variables
  float pos[2]; // position of frog
  int diseased; // flag for disease status of frog
  int hop_count; // the total number of hops the frog has done
  int hops_this_year; // the hops the frog has done since the last monsoon
	int current_cell; // the cell the frog is currently in
  int current_year; // the current year
  int population_history[300]; // the population_influx of the last 300 land cells visited
  int infection_history[500]; // the infection_level of the last 500 land cells visited
} Frog;

// needed for frog_role
void frog_initialisation(Actor *actor, void *prop);
void frog_script(Actor *actor);

// decides if a frog should have disease, based on it's id and the total number of diseased frogs
int choose_disease(Actor *actor);

#endif

