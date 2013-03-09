#ifndef __FROGS_H__
#define __FROGS_H__

#include <actormetaphor.h>
#include "extra_functions.h"
#include "../provided-functions/ran2.h"
#include "../provided-functions/frog-functions.h"

int initial_frog_count;
int max_frog_count;
Role frog_role;

typedef struct {
  long* state;
  float x;
  float y;
  int diseased;
  int hop_count;
  int population_history[300];
  int infection_history[700];
} Frog;

void frog_initialisation(Actor* actor);
void frog_script(Actor* actor);
void frog_reaction(Actor* actor, int message_type, int next_message_size);

#endif

