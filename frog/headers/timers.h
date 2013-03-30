#ifndef __TIMERS_H__
#define __TIMERS_H__

#include <actor_metaphor.h>
#include "extra_functions.h"

int max_time; // simulation will the timer's current_year is reached
double year_length; // with "wtime", sets the length of a year in seconds
int hop_limit; // with "hop", sets the hop count that will trigger a monsoon
int year_type; // defines if the year is measured with "wtime" or "hop"
extern Role timer_role; // the role of a timer

// the props for a timer actor
typedef struct {
  int frog_count; // the current total frog count
  int diseased_frog_count; // the current total diseased frogs
  double year_start; // the time the year started
  int hop_new_year; // a flag indicating that hop has started a new year
  double year_length; // the length of a year in seconds
  int year_type; // 1 if "hop", 0 if "wtime"
  int current_year; // the number of the current year
	int *cell_stats; // stores the statistics of each land cell during a monsoon
	int waiting; // flag indicating timer should wait for data
	int receive_count; // number of land cells that have currently sent data after a monsoon
} Timer;

// functions needed for timer_role
void timer_initialisation(Actor *actor, void *props);
void timer_script(Actor *actor);
void timer_encore(Actor *actor);

// true if a new year should happen
int is_new_year(Actor *actor);

#endif
