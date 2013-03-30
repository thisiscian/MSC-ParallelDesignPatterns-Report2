/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

#ifndef __AM_PROTEGES_H__
#define __AM_PROTEGES_H__

#include "am_core.h"

// Linked list of actors
struct Protege_s{
  Actor *actor;
  struct Protege_s *next;
};

Protege* _train_protege(Actor *actor);
void _add_new_protege(Actor *actor, Protege *protege_list);
void _retire_protege(Protege *retiree);
// have all proteges perform their jobs
int _help_proteges(Actor *actor);

#endif

