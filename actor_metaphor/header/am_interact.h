/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

#ifndef __AM_INTERACT_H__
#define __AM_INTERACT_H__

#include "am_core.h"
#include <mpi.h>
#include <stdlib.h>

/*Public Functions*/
void interact(Actor *actor, int interact_id, int message, int message_size, MPI_Datatype datatype, void *prop);
void talk(Actor *actor, int interact_id, int message);
void interact_with_all_proteges(Actor *actor, int message, int message_size, MPI_Datatype datatype, void *prop);
void talk_with_all_proteges(Actor *actor, int message);

/*Private Functions*/
void _be_interacted_with(Actor *actor);

#endif

