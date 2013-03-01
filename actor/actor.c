#include "actor.h"
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

Director* initialise_model()
{
  int mpi_rank, mpi_size;
  MPI_Init(NULL,NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  Director *director = initialise_director(mpi_rank, mpi_size);
  director->actor->id = mpi_rank;
  return director;
}

void finalise_model(){
  MPI_Finalize();
}

void director_start_show(Director *director)
{
  while(!director->stop)
  {
    director_direct_iteration(director);
  }
}

void director_direct_iteration(Director *director)
{
  director->actor_list->script(director->actor_list->actor);
  director->actor_list = director->actor_list->next;
}

Director* initialise_director(int rank, int size)
{
  Director* director = (Director*) malloc(sizeof(Director));
  director->actor_list = (Actor_List*) malloc(sizeof(Actor_List));

  director->actor = initialise_actor(rank, director_tasks, 0);
  director->actor_list = actor_list_new_link(director->actor);
  director->actor_list->next = director->actor_list;
  director->total_actor_count = 0;
  director->current_actor_count = 0;

  director->mpi_rank = rank;
  director->mpi_size = size;
  director->stop = 0;

  return director;
}

Actor* initialise_actor(int internal_id, void (*new_script)(Actor* actor), void* data)
{
  Actor *actor = (Actor*) malloc(sizeof(Actor));
  actor->kill = 0;
  actor->script = new_script;
  actor->id = internal_id;
  actor->props = data;
  return actor;
}

Actor_List* actor_list_new_link(Actor* new_actor)
{
  Actor_List* actor_list = (Actor_List*) malloc(sizeof(Actor_List));
  actor_list->actor = new_actor;
  actor_list->script = new_actor->script;
  return actor_list;
}

void actor_spawn(Director* director, void(*script)(Actor* actor), void* data)
{
  director->total_actor_count++; 
  director->current_actor_count++;
      printf("Director(%d): spawning actor(%d)\n", director->actor->id, director->mpi_rank+director->mpi_size*director->total_actor_count);
  Actor *actor = initialise_actor(director->mpi_rank+director->mpi_size*director->total_actor_count, script, data);

  // Link the actor_list cyclically, and insert new nodes into the next element
  Actor_List* tmp_list = director->actor_list->next;
  director->actor_list->next = actor_list_new_link(actor);
  director->actor_list->next->next = tmp_list;
  printf("Director(%d): linked list of actors created\n", director->actor->id);
}

void director_tasks(Actor* actor, void* data) {}
