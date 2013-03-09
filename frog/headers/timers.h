#ifndef __TIMERS_H__
#define __TIMERS_H__

#include <actormetaphor.h>
#include <time.h>
#include "extra_functions.h"

int max_time;
Role timer_role;

typedef struct {
  long *state;
  int frog_count;
  int diseased_frog_count;
  time_t year_start;
  int year_length;
  int current_year;
} Timer;

void timer_initialisation(Actor* actor);
void timer_script(Actor* actor);
void timer_reaction(Actor* actor, int message_type, int next_message_size);

#endif
