#include "actor.h"
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int next_id = 1;

Actor* initialise_model()
{
  int mpi_rank, mpi_size;
  MPI_Init(NULL,NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  Actor *actor = initialise_actor(mpi_rank, default_task, 0);
  return actor;
}

void finalise_model(){
  MPI_Finalize();
}

void start(Actor *actor)
{
  //printf("Actor(%d) starting\n", actor->id);
  do
  {
    actor->script(actor);
    actor_run_child_iteration(actor);
  }
  while(!actor->stop && actor->id < actor->mpi_size);
}

void actor_run_child_iteration(Actor *actor)
{
  //printf("Actor(%d) checking for children...\n", actor->id);
  while(actor->child_list != NULL)
  {
    //printf("Actor(%d) found child; telling Actor(%d) to start\n", actor->id, actor->child_list->actor->id);
    start(actor->child_list->actor);
    actor->child_list = actor->child_list->next;
  }
  
  actor->child_list = actor->list_beginning;
}

Actor* initialise_actor(int new_id, void (*new_script)(Actor* actor), void* data)
{
  Actor *actor = (Actor*) malloc(sizeof(Actor));
  actor->id = new_id;
  MPI_Comm_rank(MPI_COMM_WORLD, &(actor->mpi_rank));
  MPI_Comm_size(MPI_COMM_WORLD, &(actor->mpi_size));
  actor->kill = 0;
  actor->stop = 0;
  actor->script = new_script;
  actor->props = data;
  actor->child_list = NULL;
  return actor;
}

Actor_List* new_actor_list(Actor* new_actor)
{
  Actor_List* actor_list;
  if(!(actor_list = malloc(sizeof(Actor_List)))) return NULL;
  actor_list->next=NULL;
  actor_list->actor = new_actor;
  return actor_list;
}

Actor_List* actor_list_new_link(Actor* new_actor, Actor_List* new_list)
{
  Actor_List* actor_list = new_actor_list(new_actor);
  actor_list->next = new_list;
  return actor_list;
}

void actor_spawn(Actor* parent, void(*script)(Actor* actor), void* data)
{
  Actor *actor = initialise_actor(parent->mpi_rank+next_id*parent->mpi_size, script, data);
  next_id++;
  printf("Actor(%d) spawning actor(%d)\n", parent->id, actor->id);
  if(parent->child_list != NULL)
  {
    parent->child_list->next = actor_list_new_link(actor, parent->child_list->next);
  }
  else 
  {
    parent->child_list = actor_list_new_link(actor,NULL);
    parent->list_beginning = parent->child_list;
  }
}

void default_task(Actor* actor, void* data) {}
