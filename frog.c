#include <mpi.h>
#include <stdlib.h>
#include "actor/actor.h"

#include <stdio.h>
#include <time.h>
#include "provided-functions/ran2.h"
#include "provided-functions/frog-functions.h"

enum {  
  MONSOON,
  HOP_IN,
  FROG_SPAWN,
  FROG_SICK,
  FROG_CROAK,
  RETIRE
};

int initial_frog_count = 34;
int initial_cell_count = 16;
int max_frog_count = 100;
int max_time = 100;
int land_cell_id_counter = 0;
long state;

long* get_seed(Actor* actor);

void timer_initialisation(Actor* actor);
void timer_script(Actor* actor);
void timer_reaction(Actor* actor, int message_type, int next_message_size);

void land_cell_initialisation(Actor* actor);
void land_cell_script(Actor* actor);
void land_cell_reaction(Actor* actor, int message_type, int next_message_size);

void frog_initialisation(Actor* actor);
void land_cell_script(Actor* actor);
void frog_reaction(Actor* actor, int message_type, int next_message_size);

void (*choose_rehearse(int id))(Actor* actor);
void (*choose_script(int id))(Actor* actor);
int memory_allocation(int id);

typedef struct {
  long *state;
  int frog_count;
  int diseased_frog_count;
  time_t year_start;
  int year_length;
  int current_year;
} Timer;

typedef struct {
  long *state;
  int land_cell_id;
  int population_influx;
  int infection_level;
} Land_Cell;

typedef struct {
  long* state;
  float x;
  float y;
  int diseased;
  int hop_count;
  int population_history[300];
  int infection_history[700];
} Frog;

int main()
{
  Actor *root_actor;

  setbuf(stdout, NULL);
  root_actor = initialise_actor_model(choose_rehearse, choose_script, memory_allocation);
  read_script(root_actor); 
  finalise_actor_model();
  return 0;
}

long* get_seed(Actor* actor)
{
  if(actor->id < actor->mpi_size)
  {
    state = -1 - actor->id;
    initialiseRNG(&state);
  }
  return &state;
}

void timer_initialisation(Actor* actor)
{
  Timer *t_props = actor->props;
  t_props->frog_count = initial_frog_count;
  t_props->diseased_frog_count = 0;
  t_props->state = get_seed(actor);
  t_props->year_start= time(NULL);
  t_props->year_length = 10;
}

void timer_script(Actor* actor)
{
  int i;
  Timer *t_props = actor->props;
  if(difftime(time(NULL),t_props->year_start) > t_props->year_length)
  {
    t_props->current_year++;
    printf
    (
      "TIMER: new year (%d/%d)\n\tTotal Frog Count = %d\n\tDisease Frog Count = %d\n",
      t_props->current_year,
      max_time,
      t_props->frog_count,
      t_props->diseased_frog_count
    );
    for(i=0;i<actor->mpi_size;i++)
    {
      read_line(actor, i, MONSOON, 0);
    }
 
  }
  if(t_props->current_year > max_time)
  {
    for(i=0;i<actor->mpi_size;i++)
    {
      read_line(actor, i, RETIRE, 0);
    }
    read_line_to_all_proteges(actor, RETIRE, 0);
    actor->retire = 1;
  }
  else if(t_props->frog_count > max_frog_count)
  {
    printf("ERROR: frog count exceeded maximum (%d), exiting... \n", max_frog_count);
    for(i=0;i<actor->mpi_size;i++)
    {
      read_line(actor, i, RETIRE, 0);
    }
    read_line_to_all_proteges(actor, RETIRE, 0);
    actor->retire = 1;
  }
}

void timer_reaction(Actor* actor, int message_type, int next_message_size)
{
  Timer *t_props = actor->props;
  if(message_type == FROG_SPAWN)
  {
    t_props->frog_count++;
  }
  else if(message_type == FROG_SICK)
  {
    t_props->diseased_frog_count++;
  }
  else if(message_type == FROG_CROAK)
  {
    t_props->frog_count--;
    t_props->diseased_frog_count--;
  }
  if(message_type == RETIRE)
  {
    read_line_to_all_proteges(actor, RETIRE, 0);
    actor->retire = 1;
  }
}

void land_cell_initialisation(Actor* actor)
{
  Land_Cell *lc_props = actor->props;
  lc_props->land_cell_id = land_cell_id_counter;
  lc_props->state = get_seed(actor);
  land_cell_id_counter++;
  lc_props->population_influx=0;
  lc_props->infection_level=0;
}

void land_cell_script(Actor* actor)
{
  react_to_line(actor, land_cell_reaction);
}

void land_cell_reaction(Actor* actor, int message_type, int next_message_size)
{
  Land_Cell *lc_props = actor->props;
  if(message_type == MONSOON)
  {
    lc_props->population_influx = 0;
    lc_props->infection_level = 0;
  } 
  else if(message_type == HOP_IN)
  {
    int new_disease_value;
    lc_props->population_influx = 0;
    get_props(actor, next_message_size, MPI_INT, &new_disease_value);
    lc_props->infection_level += new_disease_value;
  }
  else if(message_type == RETIRE)
  {
    read_line_to_all_proteges(actor, RETIRE, 0);
    actor->retire = 1;
  }
}

void frog_initialisation(Actor* actor)
{
  int i;
  Frog *f_props = actor->props;

  frogHop(0, 0, &(f_props->x), &(f_props->y), f_props->state);
  f_props->hop_count=0;
  f_props->diseased=0;
  f_props->state = get_seed(actor);
  for(i=0; i<300; i++)
  {
    f_props->population_history[i] = 0;
  }
}

void frog_script(Actor* actor)
{
  float new_x, new_y;
  int i;
  int current_cell,
  int cell_stats[2]; 
  float average_cell_population = 0, average_cell_infection = 0;
  Frog *f_props = actor->props;

  react_to_line(actor, frog_reaction);

  frogHop(f_props->x, f_props->y, &new_x, &new_y, f_props->state);
  f_props->x = new_x;
  f_props->y = new_y;
  f_props->hop_count++;
  current_cell = getCellFromPosition(f_props->x, f_props->y);
  //### MESSAGE CURRENT CELL TO GET VALUES
  for(i=0; i<300; i++)
  {
    average_cell_population += f_props->population_history[i];
  }
  average_cell_population/=min(700,f_props->hop_count);
  for(i=0; i<700; i++)
  {
    average_cell_infection += f_props->infection_history[i];
  }
  average_cell_infection/=min(700,f_props->hop_count);
  if(willGiveBirth(average_cell_population, f_props->state))
  {
    Actor* baby_frog = actor_spawn(actor->mentor, frog_initialisation, frog_script, sizeof(Frog));
    Frog* baby_f_props = frog->props;
    baby_f_props->x = f_props->x;
    baby_f_props->y = f_props->y;
  };
  if(willCatchDisease(average_cell_infection)
}

void frog_reaction(Actor* actor, int message_type, int next_message_size)
{
  if(message_type == RETIRE)
  {
    read_line_to_all_proteges(actor, RETIRE, 0);
    actor->retire = 1;
  }
}

void (*choose_rehearse(int id))(Actor* actor)
{
  if(id == 0)
  {
    return timer_initialisation; 
  }
  if(id > 0 && id <= initial_cell_count)
  {
    return land_cell_initialisation;
  }
  else
  {
    return frog_initialisation;
  }
}

void (*choose_script(int id))(Actor* actor)
{
  if(id == 0)
  {
    return timer_script;
  }
  if(id > 0 && id <= initial_cell_count)
  {
    return land_cell_script;
  }
  else
  {
    return frog_script;
  }
}

int memory_allocation(int id)
{
  if(id == 0)
  {
    return sizeof(Timer);
  }
  if(id > 0 && id <= initial_cell_count)
  {
    return sizeof(Land_Cell);
  }
  else
  {
    return sizeof(Frog);
  }
}
