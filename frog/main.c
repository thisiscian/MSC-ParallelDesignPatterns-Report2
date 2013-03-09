#include <actormetaphor.h>
#include "headers/extra_functions.h"
#include "headers/frogs.h"
#include "headers/land_cells.h"
#include "headers/timers.h"

int main()
{
  // Define a lead_actor, the first actor on an mpi process
  // Note that this is a normal actor type, it is no different to any
  // other actor.
  Actor *lead_actor;
  Actor *test_actor;

  // Set up global variables
  initial_frog_count = 34;
  initial_cell_count = 16;
  max_frog_count = 100;
  max_time = 100;
  land_cell_id_counter = 0;

  // Define the lead_actor according to the functions that follow;
  lead_actor = actor_initialise_metaphor(choose_role);

  // Assign actors to the remaining initial roles
  do {
    test_actor = actor_train_protege(lead_actor, choose_role(get_next_id(lead_actor)));
  }
  while (test_actor->script != NULL);
  
  // Have the lead_actor perform it's script, and thus all protege jobs underneath it
  perform(lead_actor); 
  
  // Stop the metaphor
  actor_finalise_metaphor();
  return 0;
}
