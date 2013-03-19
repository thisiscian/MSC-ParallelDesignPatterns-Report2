/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

// Function definitions can be found in `actor.c` which should be located in
// the same folder as this file.
//
// Structs and functions are defined here that are essential to this
// implementation of the actor model.

#ifndef __PDP_ACTOR_H__
#define __PDP_ACTOR_H__

#include <mpi.h>

extern int number_of_processes;
extern int process_rank;
extern int buff_size;
// This typedef is done to allow the struct Actor_s to use Protege_s before
// it is defined
typedef struct Protege_s Protege_s;

// These typedefs are for simplicity when using the program; you need only
// define elements as `Actor` or `Protege`, not `struct Actor_s` or 
// `struct Protege_s`.
typedef struct Protege_s Protege;
typedef struct Actor_s Actor;
typedef struct Role_s Role;

struct Actor_s{
  int id;                           // unique internal identifier

  int retire;                       // flag to indicating that no more jobs
                                    // should be run
  
	int poison_pill;									// kills entire process immediately	

	int act_number;										// flag indicating which portion of the
																		// script should be read

	int sender;												// id of actor sending message currently

  void *props;                      // should point to struct containing
                                    // variables related to the actor

  void (*rehearse)();               // user defined function that runs when the
                                    // actor is declared, intended for
                                    // initialisation of props

  void (*script)();                 // user defined function containing the
                                    // jobs that this actor runs in a loop
                                    
  struct Actor_s *mentor;           // parent `Actor` of this actor

  struct Protege_s *proteges;       // linked list containing child `Actors` of this actor

  struct Protege_s *first_protege;  // points to first protege, for returning to top of list
};

// `Protege_s` is a linked list containing a child actors
struct Protege_s{
  Actor* actor;
  struct Protege_s *next;
};

// `Role` is a struct containing the various things required to implement a
// role; a rehearsal function, a script and the memory required to store props
struct Role_s{
  void (*rehearse)(Actor* actor);
  void (*script)(Actor* actor);
  int memory_required;
};

extern Role NULL_ROLE;

int is_null_role(Role role);

// Initialises the actor model, using input functions that control what the 
// initial actors on each process run
Actor* actor_initialise_metaphor(Role (*choose_roll)(int id));

// Function finalising the actor model
void actor_finalise_metaphor();

// Creates a new actor, unattached to any parent
Actor* _train_actor (Actor* mentor, int internal_id, Role role);

int peek_next_id();
int get_next_id();

// Spawns an actor attached to a specific parent
Actor* actor_train_protege (Actor* mentor, Role role);

// Creates an unattached protege
Protege* _train_protege(Actor *new_actor);

// Defines a new Protege, intended for adding a Protege to the list of Proteges
Protege* _add_protege_to_proteges(Actor *new_actor, Protege *new_list);

// Removes a Protege from the Proteges List
void _retire_protege(Actor* actor);

// Tells actor to run one iteration of their job
int perform(Actor *actor);

// Tells actors to run their proteges jobs once
int _help_proteges(Actor *actor);

// Functions that allow the sending and recieving of messages and props
void enter_dialogue(Actor* me, int you, int message_type);

void enter_dialogue_with_all_proteges(Actor* me, int message_type);
void respond_to_dialogue(Actor* actor);
void give_props(Actor* me, int you, int prop_count, MPI_Datatype datatype, void* prop);
void give_props_to_all_proteges(Actor* me, int prop_count, MPI_Datatype datatype, void* prop);
void get_props(Actor*me, int prop_count, MPI_Datatype datatype, void* prop);

// Default rehearsal and scripts
void no_script(Actor* actor);
void no_rehearse(Actor* actor);
void exit_stage(Actor* actor);

#endif
