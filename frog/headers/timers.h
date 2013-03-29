#ifndef __TIMERS_H__
#define __TIMERS_H__

#include <actor_metaphor.h>
#include "extra_functions.h"

int max_time;
double year_length;
int hop_limit;
int year_type;
extern Role timer_role;

typedef struct {
	long state;
  int frog_count;
  int diseased_frog_count;
  double year_start;
  int hop_new_year;
  double year_length;
  int year_type;
  int current_year;
	int *cell_stats;
	int waiting;
	int receive_count;
} Timer;

void timer_initialisation(Actor *actor, void *props);
void timer_script(Actor *actor);
void timer_encore(Actor *actor);

int is_new_year(Actor *actor);

#endif
