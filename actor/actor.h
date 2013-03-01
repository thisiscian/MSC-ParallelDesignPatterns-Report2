#ifndef __PDP_ACTOR_H__
#define __PDP_ACTOR_H__

typedef struct Actor_List_s Actor_List_s;

struct Actor_s{
  int id;             // unique id by which a variable can be identified
  int mpi_rank;
  int mpi_size;
  int kill;           // flag to kill 
  int stop;
  void (*rehearse)(); // initialisation function pointer
  void (*script)(struct Actor_s* actor);   // main loop function pointer
  void (*retire)();   // kill actor
  void *props;        // variables the actor owns
  struct Actor_List_s *child_list;
  struct Actor_List_s *list_beginning;
};
typedef struct Actor_s Actor;

struct Actor_List_s{
  Actor* actor;
  struct Actor_List_s *next;
};
typedef struct Actor_List_s Actor_List;

Actor* initialise_model();
void finalise_model();

void actor_run_child_iteration(Actor *actor);
void start(Actor *actor);

Actor* initialise_actor(int internal_id, void (*script)(), void* data);
Actor_List* new_actor_list(Actor *new_actor);
Actor_List* actor_list_new_link(Actor *new_actor, Actor_List *new_list);
void actor_kill(Actor *actor);
void actor_spawn_on_director(int director_id);
void actor_spawn(Actor* actor, void (*script)(Actor* actor), void* data);
void default_task();

#endif
