#ifndef __PDP_ACTOR_H__
#define __PDP_ACTOR_H__

struct Actor_s{
  int id;             // unique id by which a variable can be identified
  int kill;           // flag to kill 
  void (*rehearse)(); // initialisation function pointer
  void (*script)(struct Actor_s* actor);   // main loop function pointer
  void (*retire)();   // kill actor
  void *props;        // variables the actor owns
};
typedef struct Actor_s Actor;

struct Actor_List_s{
  Actor* actor;
  void (*script)(Actor* actor);
  struct Actor_List_s *next;
};
typedef struct Actor_List_s Actor_List;

typedef struct {
  Actor *actor;
  int mpi_rank;
  int mpi_size;
  int total_actor_count;
  int current_actor_count;
  int stop;
  Actor_List *actor_list ;
} Director;


Director* initialise_model();
void finalise_model();

void director_direct_iteration(Director *director);
void director_start_show(Director *director);

Actor* initialise_actor(int internal_id, void (*script)(), void* data);
Director* initialise_director(int mpi_rank, int mpi_size);
Actor_List* actor_list_new_link(Actor *);
void actor_kill(Actor *actor);
void actor_spawn_on_director(int director_id);
void actor_spawn(Director* director, void (*script)(Actor* actor), void* data);
void director_tasks();

#endif
