/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

#include "../header/am_core.h"
#include "../header/am_proteges.h"
#include "../header/am_role.h"
#include "../header/am_misc.h"

Actor* actor_train_protege (Actor *mentor, Role role){
  Actor *actor = _train_actor(mentor, get_next_id(), role);
  if(mentor->proteges != NULL){
    _add_new_protege(actor, mentor->proteges);
  }
  else{
    mentor->proteges = _train_protege(actor);
  }
  return actor;
}

int perform(Actor *actor){
	int i,performances = 0;
  do{
    if(actor->mentor == NULL){
      performances = 0;
    }
    
		if(actor->poison_pill){
			for(i=0;i<number_of_processes;i++){
				talk(actor,i,-1);
			}
			return 0;
		}
    if(!actor->retire){
			_be_interacted_with(actor);
      actor->script(actor);
      performances++;
    }
		performances += _help_proteges(actor);
  }
  while(performances > 0 && actor->mentor == NULL);
  return performances;
}

Actor *_train_actor (Actor *new_mentor, int new_id, Role role) {
  Actor *actor = (Actor*) malloc(sizeof(Actor));
  actor->id = new_id;
  actor->retire = 0;
  actor->mentor = new_mentor;
  actor->proteges = NULL;
	actor->sender = -1;
	actor->poison_pill = 0;
	if(!is_null_role(role)){
		actor->script = role.script;
  	actor->rehearse = role.rehearse;
  	actor->props = (void*) malloc(role.memory_required);
  	actor->rehearse(actor);
	}
	else{
		actor->script = no_script;
		actor->rehearse = no_rehearse;
  	actor->props = malloc(0);
	}
  return actor;
}
