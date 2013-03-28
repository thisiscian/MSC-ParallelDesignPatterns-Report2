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

void _retire_protege(Protege *retiree){
  _retire_actor(retiree->actor);
  free(retiree);
	return;
}

int _help_proteges(Actor *actor) {
  int protege_count = 0;
	Protege *protege = actor->proteges;
	Protege *prev = NULL;
	Protege *tmp;
  while(protege != NULL && !actor->poison_pill){
    if(perform(protege->actor)){
      protege_count++;
      prev = protege;
      protege = protege->next;
    }
    else{
      tmp = protege;
      protege = protege->next;
      prev->next = protege;
      _retire_protege(tmp);
    }
  }
  return protege_count;
}

