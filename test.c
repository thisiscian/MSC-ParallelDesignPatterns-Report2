#include <mpi.h>
#include <stdio.h>
#include "actor/actor.h"

void act_init();
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

  new_actor.actor->init = act_init;
  new_actor.actor->start = act_start;
  new_actor.actor->stop = act_stop;

  new_actor.actor->init();
  new_actor.actor->start();
  new_actor.actor->stop();
  new_actor.actor->kill(tis);
  actors_finalize();
  
  return 0;
}

void act_init()
{
  printf("init\n");
}
void act_start()
{
  printf("start\n");
}
void act_stop()
{
  printf("stop\n");
}
