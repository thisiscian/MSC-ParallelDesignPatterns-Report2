#ifndef __PDP_ACTOR_H__
#define __PDP_ACTOR_H__

typedef struct Actor_List_s Actor_List_s;

struct Actor_s{
  int id;             // unique id by which the actor can be identified
  int mpi_rank;       // rank of mpi process
  int mpi_size;       // number of mpi processes
  int stop;           // flag to stop
  void (*rehearse)(); // custom function that runs before the actor starts
  void (*script)();   // custom function that actor runs every iteration
  void *props;        // variables the actor owns
  struct Actor_List_s *child_list; // linked list containing children of actor
  struct Actor_List_s *list_beginning; // copy of first element of list
};
typedef struct Actor_s Actor;

struct Actor_List_s{
  Actor* actor;
  struct Actor_List_s *next;
};
typedef struct Actor_List_s Actor_List;


// Functions that initialise and finalise the model
Actor* initialise_model();
void finalise_model();

// Functions that initialise and spawn actors
Actor* _train_actor
(
  int internal_id,
  void (*rehearse)(Actor* actor),
  void (*script)(Actor* actor),
  int mem_size
);

void actor_spawn
(
  Actor* actor,
  void (*rehearse)(Actor* actor),
  void (*script)(Actor* actor),
  int mem_size
);

// Functions that generate linked list of Actors
Actor_List* _train_actor_network(Actor *new_actor);
Actor_List* _link_actor_network(Actor *new_actor, Actor_List *new_list);
void _remove_from_list(Actor* actor);
void _retire_actor(Actor* actor);
// Functions that allow actor and children to read their scripts, and stop
void read_script(Actor *actor);
void _help_understudies(Actor *actor);
void stop(Actor *actor);

// Default rehearsal and scripts
void no_script();
void no_rehearsal();
void exit_stage(Actor* actor);

#endif
