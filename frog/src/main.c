#include <actor_metaphor.h>
#include "../headers/extra_functions.h"
#include "../headers/frogs.h"
#include "../headers/land_cells.h"
#include "../headers/timers.h"

int main(int argc, char* argv[]){
  Actor *lead_actor;
  Actor *test_actor;

  // exit if command line arguments are incorrect
	if(collect_input(argc, argv)) {
		printf("Exiting...\n");
		return 1;
	}

  lead_actor = actor_initialise_metaphor(choose_role);

	while (peek_next_id() <= initial_frog_count + cell_count){
    test_actor = actor_train_protege(lead_actor, choose_role(peek_next_id()), NULL);
  }
  perform(lead_actor); 
  actor_finalise_metaphor(lead_actor);
  return 0;
}
