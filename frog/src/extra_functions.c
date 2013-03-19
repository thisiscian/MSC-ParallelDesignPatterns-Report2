#include "../headers/extra_functions.h"

// Standard min function definition
int min(int a, int b)
{
  return a<=b?a:b;
}

// Returns the state of the current process; if the actor is one of the 
// lead_actors (i.e. first actor on the mpi process), then it sets the
// state for that process
long* get_seed(Actor* actor)
{
  if(actor->id < number_of_processes)
  {
    state = -1-(actor->id);
    initialiseRNG(&state);
  }
  return &state;
}

Role choose_role(int id)
{
  if(id == 0)
  {
    return timer_role;
  }
  else if(id > 0 && id <= initial_cell_count)
  {
    return land_cell_role;
  }
  else if(id > initial_cell_count && id <= initial_cell_count+initial_frog_count)
  {
    return frog_role;
  }
  else
  {
    return NULL_ROLE;
  }
}
