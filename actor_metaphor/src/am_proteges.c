/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

#include "../header/am_proteges.h"

// train a new protege
Protege* _train_protege(Actor *actor){
  Protege* actor_list = malloc(sizeof(Protege));
  actor_list->next=NULL;
  actor_list->actor = actor;
  return actor_list;
}

// add a protege to an existing list
void _add_new_protege(Actor *actor, Protege *protege_list){
	Protege *protege = protege_list;
	while(protege->next != NULL){
		protege = protege->next;
	}
	protege->next = _train_protege(actor);
	return;
}

// retires protege
void _retire_protege(Protege *retiree){
  _retire_actor(retiree->actor);
  free(retiree);
	return;
}

// allow proteges to read scripts
int _help_proteges(Actor *actor) {
  int protege_count = 0;
	Protege *protege = actor->proteges;
	Protege *prev = NULL;
	Protege *tmp;
  // stop if out of proteges or have received poison pill
  while(protege != NULL && !actor->poison_pill){
    if(perform(protege->actor)){
      // if protege had work, increment to next one
      protege_count++;
      prev = protege;
      protege = protege->next;
    } else {
      // if protege had no work, retire and increment
      tmp = protege;
			if(prev != NULL) {
      	protege = protege->next;
		    prev->next = protege;
	      _retire_protege(tmp);
			} else {
				actor->proteges = actor->proteges->next;
				protege = actor->proteges;	
	      _retire_protege(tmp);
			}
    }
  }
  return protege_count;
}

