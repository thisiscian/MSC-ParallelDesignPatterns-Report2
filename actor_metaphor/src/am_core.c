/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

#include "../header/am_core.h"
#include "../header/am_proteges.h"
#include "../header/am_role.h"
#include "../header/am_misc.h"
#include "../header/am_interact.h"

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

Actor *_train_actor (Actor *new_mentor, int new_id, Role role, void *props) {
  Actor *actor = (Actor*) malloc(sizeof(Actor));
  actor->id = new_id;
  actor->retire = 0;
  actor->mentor = new_mentor;
  actor->proteges = NULL;
	actor->sender = -1;
	actor->poison_pill = 0;
  actor->sent_props = NULL;
	if(!is_null_role(role)){
		actor->script = role.script;
  	actor->rehearse = role.rehearse;
		actor->encore = role.encore;
  	actor->props = (void*) malloc(role.memory_required);
		memset(actor->props, 0, role.memory_required);
  	actor->rehearse(actor, props);
	}
	else{
		actor->script = no_script;
		actor->rehearse = no_rehearse;
		actor->encore = no_encore;
  	actor->props = malloc(0);
	}
  return actor;
}

void _retire_actor(Actor *actor){
  Protege *tmp;
  while(actor->proteges != NULL){
    tmp = actor->proteges;
    actor->proteges = actor->proteges->next;
    _retire_protege(tmp);
  }
	actor->encore(actor);
  free(actor->sent_props);
  free(actor->props);
  actor->props = NULL;
  free(actor);
  actor = NULL;
  return;
}
