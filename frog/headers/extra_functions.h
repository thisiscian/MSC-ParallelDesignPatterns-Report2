#ifndef __FROG_EXTRA_H__
#define __FROG_EXTRA_H__

#include <stdio.h>
#include <math.h>
#include "../provided-functions/ran2.h"
#include "frogs.h"
#include "land_cells.h"
#include "timers.h"

long state;

// List of possible messages to send; this just makes message passing a bit
// clearer for the user, it's not necessary
enum {  
  MONSOON,
  HOP_IN,
  FROG_SPAWN,
  FROG_SICK,
  FROG_CROAK,
  RETIRE
};


long* get_seed(Actor* actor);
Role choose_role(int id);
int min(int a, int b);

#endif
