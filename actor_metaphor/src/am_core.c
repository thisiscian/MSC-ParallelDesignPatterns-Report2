/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

#include "../header/am_core.h"
#include "../header/am_proteges.h"
#include "../header/am_role.h"
#include "../header/am_misc.h"
#include "../header/am_interact.h"

// use this to spawn new actors on the current process
Actor* actor_train_protege (Actor *mentor, Role role, void *props){
  Actor *actor = _train_actor(mentor, get_next_id(), role, props);
  if(mentor->proteges != NULL){
    _add_new_protege(actor, mentor->proteges);
  }
  else{
    mentor->proteges = _train_protege(actor);
  }
  return actor;
}

// runs job as supplied in script
int perform(Actor *actor){
	int i,performances;
  do{
    performances = 0; //counts the jobs that have been done this iteration
    // if poison_pill has been issued, kill everyone
		if(actor->poison_pill){
			for(i=0;i<number_of_processes;i++){
				talk(actor,i,-1);
			}
			return 0;
		}

    // if not retired, receive messages and read script
    if(!actor->retire){
			_be_interacted_with(actor);
      actor->script(actor);
      performances++;
    }
    // help proteges with their scripts
		performances += _help_proteges(actor);
  }
  while(performances > 0 && actor->mentor == NULL);
  // if you are the lead actor, iterate until there are no more jobs being done
  return performances;
}

// spawns actor with a given id and role
Actor *_train_actor (Actor *new_mentor, int new_id, Role role, void *props) {
  Actor *actor = (Actor*) malloc(sizeof(Actor));

  // initialises actor variables
  actor->id = new_id;
  actor->retire = 0;
  actor->mentor = new_mentor;
  actor->proteges = NULL;
	actor->sender = -1;
	actor->poison_pill = 0;
  actor->sent_props = NULL;

	if(!is_null_role(role)){
    // if not null role, assign role
		actor->script = role.script;
  	actor->rehearse = role.rehearse;
		actor->encore = role.encore;
  	actor->props = (void*) malloc(role.memory_required);
		memset(actor->props, 0, role.memory_required); //initialise data to 0
  	actor->rehearse(actor, props);
	}
	else{
    // if role is null_role, then have empty jobs
		actor->script = no_script;
		actor->rehearse = no_rehearse;
		actor->encore = no_encore;
  	actor->props = malloc(0);
	}
  return actor;
}

// removes actor from work, frees proteges and allocated memory
void _retire_actor(Actor *actor){
  Protege *tmp;
  
  // for each protege...
  while(actor->proteges != NULL){
    tmp = actor->proteges;
    actor->proteges = actor->proteges->next;
    _retire_protege(tmp);
  }
	actor->encore(actor);
  free(actor->sent_props);
  free(actor->props);
  free(actor);
  actor = NULL; // make sure no hanging pointers
  return;
}
