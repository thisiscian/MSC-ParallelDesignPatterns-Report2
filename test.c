#include <mpi.h>
#include <stdio.h>
#include "actor/actor.h"

void act_start(Actor* actor);
void act_stop(Actor* actor);

typedef struct {
  int one;
  double two;
} my_prop;

int main()
{
  Actor *root_actor = initialise_model();
  root_actor->script = act_start;
  my_prop actor_data = {1, 2.0};
  actor_spawn(root_actor, act_start, 0);
  start(root_actor); 
  finalise_model();
  return 0;
}

void act_start(Actor* actor)
{
  printf("Actor(%d) reading from script\n", actor->id);
}

void act_stop(Actor* actor)
{
  printf("Actor(%d) stopping now\n", actor->id);
}
