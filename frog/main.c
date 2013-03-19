#include <actormetaphor.h>
#include "headers/extra_functions.h"
#include "headers/frogs.h"
#include "headers/land_cells.h"
#include "headers/timers.h"

int main(int argc, char* argv[])
{
  // Define a lead_actor, the first actor on an mpi process
  // Note that this is a normal actor type, it is no different to any
  // other actor.
  Actor *lead_actor;
  Actor *test_actor;

	if(argc == 7)
	{
		initial_frog_count = atoi(argv[1]);
		initial_cell_count = atoi(argv[2]);
 		max_frog_count = atoi(argv[3]);
  	max_time = atoi(argv[4]);
		year_length = atoi(argv[5]);
		buff_size = 10000*sizeof(double);
	}
	else
	{
//  	initial_frog_count = 10;
//  	initial_cell_count = 16;
// 		max_frog_count = 10;
//  	max_time = 2;
//		year_length = 1;
//		buff_size = 10000*sizeof(double);
  	initial_frog_count = 34;
  	initial_cell_count = 16;
 		max_frog_count = 100;
  	max_time = 100;
		year_length = 1;
		buff_size = 10000*sizeof(double);
	}

  // Define the lead_actor according to the functions that follow;
  lead_actor = actor_initialise_metaphor(choose_role);
  // Assign actors to the remaining initial roles
	int id = peek_next_id();
	while (id <= initial_frog_count + initial_cell_count)
	{
    test_actor = actor_train_protege(lead_actor, choose_role(id));	
		id = peek_next_id();
  }
  // Have the lead_actor perform it's script, and thus all protege jobs underneath it
  perform(lead_actor); 
  
  // Stop the metaphor
  actor_finalise_metaphor();
  return 0;
}
