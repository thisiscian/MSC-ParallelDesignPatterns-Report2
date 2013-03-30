/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

#ifndef __AM_MISC_H__
#define __AM_MISC_H__

#include "am_core.h"
#include <mpi.h>

extern int next_id; // ensures unique ids
extern int number_of_processes; // number of MPI processes
extern int process_rank; // current MPI process
extern int buff_size; // size of MPI buffer used for sending messages
extern Role NULL_ROLE; // an empty role

/*Public Functions*/
// start and stop model
Actor *actor_initialise_metaphor(Role (*choose_roll)(int id));
void actor_finalise_metaphor(Actor *actor);

int peek_next_id();
int get_next_id();

// needed for null role
void no_script(Actor *actor);
void no_rehearse(Actor *actor);
void no_encore(Actor *actor);

#endif

