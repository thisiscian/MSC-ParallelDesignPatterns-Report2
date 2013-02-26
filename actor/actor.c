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

void actor_kill(this)
{
  this.kill_now = 1;
  printf("being killed\n");
}

Actor* initActor()
{
  Actor *actor = (Actor*) malloc(sizeof(Actor));
  MPI_Comm_rank(MPI_COMM_WORLD, &(actor->mpi_rank));
  MPI_Comm_size(MPI_COMM_WORLD, &(actor->mpi_size));
  actor->kill_now = 0;
  actor->kill = actor_kill;
}
