/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

#ifndef __AM_ROLE_H__
#define __AM_ROLE_H__

#include "am_core.h"

extern Role NULL_ROLE;

// defines what an actors type is
struct Role_s {
  void (*rehearse)(Actor* actor, void *props);
  void (*script)(Actor *actor);
  void (*encore)(Actor *actor);
  int memory_required;
};

/*Public Functions*/
// checks to see if given role is null role
int is_null_role(Role role);

#endif
