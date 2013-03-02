// Functions used to call the Actor Model using MPI
// Each MPI process spawns at least one actor
// However, each MPI process can have any amount of actors, by having the
// initial actor spawn more
//

#include "actor.h"
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

// global variable currently needed for unique ID of actors
int next_id = 1;

// Actor* INITIALISE_MODEL
// Initialise MPI and create one actor per process, who's id is the rank of the
// host process.
Actor* initialise_model()
{
  int mpi_rank;
  MPI_Init(NULL,NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  Actor *actor = _train_actor(mpi_rank, no_rehearsal, no_script, 0);
  return actor;
}

// void FINALISE_MODEL
// finalise MPI and the actor model
void finalise_model(){
  MPI_Finalize();
}

// void START
// Starts a particular actor doing their normal work. This consists of reading
// their 'script' and then starting any children it may have. Special case for
// the first actors on an mpi process; they keep running until they are told
// to stop_f. Other actors only run their start process once .
void read_script(Actor *actor)
{
  printf("Actor(%d) reading script\n", actor->id);
  do
  {
    actor->script(actor);
    _help_understudies(actor);
  }
  while(!actor->stop && actor->id < actor->mpi_size);
}

// void ACTOR_RUN_CHILD_ITERATION
// runs through each child of an actor and tells it to start
void _help_understudies(Actor *actor)
{
  while(actor->child_list != NULL)
  {
    if(actor->child_list->actor->stop)
    {
      _remove_from_list(actor);
    }
    read_script(actor->child_list->actor);
    actor->child_list = actor->child_list->next;
  }
  actor->child_list = actor->list_beginning;
}

// void INITIALISE_ACTOR
// initialise an actor with no children, and a given id, script and prop set
Actor* _train_actor
(
  int new_id,
  void (*new_rehearse)(Actor* actor),
  void (*new_script)(Actor* actor),
  int mem_size
)
{
  Actor *actor = (Actor*) malloc(sizeof(Actor));
  actor->id = new_id;
  MPI_Comm_rank(MPI_COMM_WORLD, &(actor->mpi_rank));
  MPI_Comm_size(MPI_COMM_WORLD, &(actor->mpi_size));
  actor->stop = 0;
  actor->script = new_script;
  actor->rehearse = new_rehearse;
  actor->props = malloc(mem_size);
  actor->child_list = NULL;
  actor->list_beginning = NULL;
  return actor;
}

// Actor_List* NEW_ACTOR_LIST
// creates a new Actor_List which points to nothing, but has an actor
Actor_List* _train_actor_network(Actor* new_actor)
{
  Actor_List* actor_list;
  if(!(actor_list = malloc(sizeof(Actor_List)))) return NULL;
  actor_list->next=NULL;
  actor_list->actor = new_actor;
  return actor_list;
}

// Actor_List* ACTOR_LIST_NEW_LINK
// creates a new Actor_List which links to another list
Actor_List* _link_actor_network(Actor* new_actor, Actor_List* new_list)
{
  Actor_List* actor_list = _train_actor_network(new_actor);
  actor_list->next = new_list;
  return actor_list;
}

void _remove_from_list(Actor* actor)
{
  Actor_List* tmp_list = actor->child_list;
  actor->child_list = actor->child_list->next;
  if(tmp_list == actor->list_beginning)
  {
    actor->list_beginning == actor->child_list;
  }
  _retire_actor(tmp_list->actor);
  free(tmp_list);
}

void _retire_actor(Actor* actor)
{
  free(actor);
}

// void ACTOR_SPAWN_WITH_ENCORE
// spawns a new actor, with a unique id
void actor_spawn
(
  Actor* parent,
  void(*rehearse)(Actor* actor),
  void(*script)(Actor* actor),
  int mem_size
)
{
  Actor *actor = _train_actor(parent->mpi_rank+next_id*parent->mpi_size, rehearse, script, mem_size);
  next_id++;
  printf("Actor(%d) spawning actor(%d)\n", parent->id, actor->id);
  actor->rehearse(actor);
  if(parent->child_list != NULL)
  {
    parent->child_list->next = _link_actor_network(actor, parent->child_list->next);
  }
  else 
  {
    parent->child_list = _link_actor_network(actor,NULL);
    parent->list_beginning = parent->child_list;
  }
}

// void NO_REHEARSAL
// No initialisation for this actor
void no_rehearsal(Actor* actor) {}

// void NO_SCRIPT
// No task for this actor
void no_script(Actor* actor) {}
