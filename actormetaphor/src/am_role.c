/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

#include "../header/am_role.h"

Role NULL_ROLE = {NULL,NULL,0};

int is_null_role(Role role)
{
	int boole = role.script == NULL_ROLE.script;
	boole &= role.rehearse == NULL_ROLE.rehearse;
	boole &= role.memory_required == NULL_ROLE.memory_required;
	return boole;
}
