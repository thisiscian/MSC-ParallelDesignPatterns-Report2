/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

#include "../header/am_role.h"

Role NULL_ROLE = {NULL,NULL,NULL,0};

// checks to see if role is the null role
int is_null_role(Role role)
{
	int boole = role.script == NULL_ROLE.script;
	boole ^= role.rehearse == NULL_ROLE.rehearse;
	boole ^= role.encore == NULL_ROLE.encore;
	boole ^= role.memory_required == NULL_ROLE.memory_required;
	return boole;
}
