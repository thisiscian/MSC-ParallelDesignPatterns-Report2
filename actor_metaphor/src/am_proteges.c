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

void _retire_protege(Actor *actor, Protege *protege){
	Protege *proteges = actor->proteges;
	if(actor->proteges == protege)
	{
		actor->proteges = protege->next;
	}
	else
	{
		while(proteges->next != protege)
		{
			if(proteges->next == NULL)
			{
				printf("Given protege is not a member of this list!");
				return;
			}
			proteges = proteges->next;
		}
		proteges->next = protege->next;
		return;
			
	}
  actor->proteges = actor->proteges->next;
	return;
}

int _help_proteges(Actor *actor) {
  int protege_count = 0;
	int test = 0;
	Protege *protege = actor->proteges;
	Protege *tmp;
  while(protege != NULL && !actor->poison_pill){
		test = perform(protege->actor);	
    if(test){
      protege_count++;
      protege = protege->next;
    }
    else{
			tmp = protege->next;
      _retire_protege(actor, protege);
			protege = tmp;
    }
  }
  return protege_count;
}

