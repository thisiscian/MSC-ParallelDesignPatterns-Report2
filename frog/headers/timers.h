#ifndef __TIMERS_H__
#define __TIMERS_H__

#include <actormetaphor.h>
#include "extra_functions.h"

int max_time;
double year_length;
extern Role timer_role;

typedef struct {
  long *state;
  int frog_count;
  int diseased_frog_count;
  double year_start;
  double year_length;
  int current_year;
} Timer;

void timer_initialisation(Actor* actor);
void timer_script(Actor* actor);

#endif
