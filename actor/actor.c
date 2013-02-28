#include "actor.h"
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

void actors_init()
{
  MPI_Init(NULL,NULL);
}

void actors_finalize(){
  MPI_Finalize();
}

void actor_kill(Actor *actor)
{
  actor->kill = 1;
}

void actor_start(Actor *actor)
{

}

Director* initDirector() {
  Director* director = (Director*) malloc(sizeof(Director));
  director.actor = initActor();
  MPI_Comm_rank(MPI_COMM_WORLD, &(director->mpi_rank));
  MPI_Comm_size(MPI_COMM_WORLD, &(director->mpi_size));
}

Actor* initActor()
{
  Actor *actor = (Actor*) malloc(sizeof(Actor));
  actor->kill = 0;
  return actor;
}

/* Could be interesting for MPI structs
  MPI_Datatype mystruct;
  int arr1[3] = {1,1,1};
  int arr2[3] = {0, sizeof(int), sizeof(int)+sizeof(double)};
  MPI_Datatype arr3[3] = {MPI_INT, MPI_DOUBLE,MPI_INT};
  MPI_Type_create_struct(3, arr1, arr2, arr3, &mystruct);
*/

