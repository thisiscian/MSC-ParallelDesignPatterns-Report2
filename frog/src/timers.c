#include "../headers/timers.h"

Role timer_role = {timer_initialisation, timer_script, sizeof(Timer)};

// Function which initialises the Timing actor
void timer_initialisation(Actor* actor)
{
  Timer *t_props = actor->props;
  t_props->frog_count = initial_frog_count;
  t_props->diseased_frog_count = 0;
  t_props->state = get_seed(actor);
  t_props->year_start= time(NULL);
  t_props->year_length = 10;
}

// The job that the Timing actor runs; when the year is over it prints
// information about the state of the system, if the run time is up, it 
// messages the lead_actors of all systems to
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
      enter_dialogue(actor, i, MONSOON, 0);
    }
 
  }
  if(t_props->current_year > max_time)
  {
    for(i=0;i<actor->mpi_size;i++)
    {
      enter_dialogue(actor, i, RETIRE, 0);
    }
    enter_dialogue_with_all_proteges(actor, RETIRE, 0);
    actor->retire = 1;
  }
  else if(t_props->frog_count > max_frog_count)
  {
    printf("ERROR: frog count exceeded maximum (%d), exiting... \n", max_frog_count);
    for(i=0;i<actor->mpi_size;i++)
    {
      enter_dialogue(actor, i, RETIRE, 0);
    }
    enter_dialogue_with_all_proteges(actor, RETIRE, 0);
    actor->retire = 1;
  }
}


// Function that defines how the Timing actor will react to dialogue messages
void timer_reaction(Actor* actor, int message_type, int next_message_size)
{
  Timer *t_props = actor->props;
  if(message_type == FROG_SPAWNED)
  {
    t_props->frog_count++;
  }
  else if(message_type == FROG_SICK)
  {
    t_props->diseased_frog_count++;
  }
  else if(message_type == FROG_CROAKED)
  {
    t_props->frog_count--;
    t_props->diseased_frog_count--;
  }
  if(message_type == RETIRE)
  {
    enter_dialogue_with_all_proteges(actor, RETIRE, 0);
    actor->retire = 1;
  }
}

