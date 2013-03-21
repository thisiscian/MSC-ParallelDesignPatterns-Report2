#include <actor_metaphor.h>
#include "../headers/extra_functions.h"
#include "../headers/frogs.h"
#include "../headers/land_cells.h"
#include "../headers/timers.h"

int main(int argc, char* argv[])
{
  Actor *lead_actor;
  Actor *test_actor;

	if(collect_input(argc, argv)) {
		printf("Exiting...\n");
		return 1;
	}

  // Define the lead_actor according to the functions that follow;
  lead_actor = actor_initialise_metaphor(choose_role);
  // Assign actors to the remaining initial roles
	while (peek_next_id() <= initial_frog_count + cell_count)
	{
    test_actor = actor_train_protege(lead_actor, choose_role(peek_next_id()), NULL);
  }
  // Have the lead_actor perform it's script, and thus all protege jobs underneath it
  perform(lead_actor); 
  // Stop the metaphor
  actor_finalise_metaphor(lead_actor);
  return 0;
}
