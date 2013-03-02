#ifndef __PDP_ACTOR_H__
#define __PDP_ACTOR_H__

typedef struct Protege_s Protege_s;
typedef struct Protege_s Protege;
typedef struct Actor_s Actor;

struct Actor_s{
  int id;             // unique id by which the actor can be identified
  int mpi_rank;       // rank of mpi process
  int mpi_size;       // number of mpi processes
  int retire;         // flag to retire
  void (*rehearse)(); // custom function that runs before the actor starts
  void (*script)();   // custom function that actor runs every iteration
  void *props;        // variables the actor owns
  struct Actor_s *mentor; //parent of actor
  struct Protege_s *proteges; // linked list containing children of actor
  struct Protege_s *first_protege; // copy of first element of list
};

struct Protege_s{
  Actor* actor;
  struct Protege_s *next;
};

// Functions that initialise and finalise the model
Actor* initialise_actor_model
(
  void (*choose_rehearse_function(int id))(Actor *actor),
  void (*choose_script_function(int id))(Actor* actor),
  int (*choose_amount_of_memory_to_allocate)(int id)
);
void finalise_actor_model();

// Functions that initialise and spawn actors
Actor* _train_actor
(
  Actor* mentor,
  int internal_id,
  void (*rehearse)(Actor* actor),
  void (*script)(Actor* actor),
  int mem_size
);

Actor* actor_spawn
(
  Actor* mentor,
  void (*rehearse)(Actor* actor),
  void (*script)(Actor* actor),
  int mem_size
);

// Functions that generate linked list of Actors
Protege* _train_protege(Actor *new_actor);
Protege* _add_protege_to_proteges(Actor *new_actor, Protege *new_list);
void _remove_from_list(Actor* actor);
void _retire_actor(Actor* actor);
// Functions that allow actor and children to read their scripts, and retire
int read_script(Actor *actor);
int _help_understudies(Actor *actor);

// Default rehearsal and scripts
void no_script();
void no_rehearse();
void exit_stage(Actor* actor);

#endif
