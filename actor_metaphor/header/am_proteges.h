/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

#ifndef __AM_PROTEGES_H__
#define __AM_PROTEGES_H__

#include "am_core.h"

struct Protege_s{
  Actor *actor;
  struct Protege_s *next;
};

Protege* _train_protege(Actor *actor);
void _add_new_protege(Actor *actor, Protege *protege_list);
void _retire_protege(Protege *retiree);
int _help_proteges(Actor *actor);

#endif

