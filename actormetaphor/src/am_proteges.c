/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

#include "../header/am_proteges.h"

Protege* _train_protege(Actor *actor){
  Protege* actor_list = malloc(sizeof(Protege));
  actor_list->next=NULL;
  actor_list->actor = actor;
  return actor_list;
}

void _add_new_protege(Actor *actor, Protege *protege_list){
	Protege *protege = protege_list;
	while(protege->next != NULL){
		protege = protege->next;
	}
	protege->next = _train_protege(actor);
	return;
}

void _retire_protege(Actor *actor){
  Protege* retiree = actor->proteges;
  actor->proteges = actor->proteges->next;
	return;
}

int _help_proteges(Actor *actor) {
  int protege_count = 0;
  int current_protege = 0;
	Protege *protege = actor->proteges;
  while(protege != NULL && !actor->poison_pill){
    if(perform(protege->actor)){
      protege_count++;
      protege = protege->next;
    }
    else{
      _retire_protege(actor);
    }
  }
  return protege_count;
}

