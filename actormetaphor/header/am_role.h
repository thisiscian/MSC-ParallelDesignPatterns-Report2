/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

#ifndef __AM_ROLE_H__
#define __AM_ROLE_H__

#include "am_core.h"

extern Role NULL_ROLE;

struct Role_s {
  void (*rehearse)();
  void (*script)(Actor *actor);
  int memory_required;
};


/*Public Functions*/
int is_null_role(Role role);

#endif
