#include <mpi.h>
#include <stdio.h>
#include "actor/actor.h"

void act_start();
void act_stop();

typedef struct {
  Actor *actor;
  int val_one;
  int val_two;
} New_actor;

int main()
{
  actors_init();
  New_actor new_actor;
  new_actor.actor = initActor();

  new_actor.actor->start = act_start;
  new_actor.actor->stop = act_stop;

  new_actor.actor->start();
  new_actor.actor->stop();
  actors_finalize();
  
  return 0;
}

void act_start(New_actor *new_actor)
{
  new_actor.actor->start();
}

void act_stop()
{
  printf("stop\n");
}
