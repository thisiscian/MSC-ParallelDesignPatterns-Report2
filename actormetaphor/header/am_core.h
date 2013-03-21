/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

#ifndef __AM_CORE_H__
#define __AM_CORE_H__

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct Role_s Role;
typedef struct Protege_s Protege;
typedef struct Actor_s Actor;

struct Actor_s{
  int id;
  int retire;
	int poison_pill;
	int act_number;
	int sender;
	int last_message_size;
	void *sent_props;
  void *props;
  void (*rehearse)();
  void (*script)(); 
  Actor *mentor;
  Protege *proteges;
};

/*Public Functions*/
Actor* actor_train_protege (Actor *mentor, Role role);
int perform(Actor *actor);

/*Private Functions*/
Actor *_train_actor (Actor *mentor, int internal_id, Role role);

#endif
