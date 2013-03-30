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

// the actor struct
struct Actor_s{
  int id; // the unique id of an actor
  int retire; // flag to stop working
	int poison_pill; // flag to kill entire session

	int act_number; // last message sent
	int sender; // id of actor sending last message
	int last_message_size; // size of data sent with last message
	void *sent_props; // data sent with last message

  void *props; // variables owned by actor
  void (*rehearse)(); // initialisation function
  void (*script)(); // job to be run
  void (*encore)(); // finalisation function

  Actor *mentor; // parent of actor
  Protege *proteges; //children of actor
};

/*Public Functions*/
//spawn a new actor
Actor* actor_train_protege (Actor *mentor, Role role, void *props);

// read from script (run job)
int perform(Actor *actor);

/*Private Functions*/
Actor* _train_actor (Actor *mentor, int internal_id, Role role, void *props);
void _retire_actor(Actor *actor);

#endif
