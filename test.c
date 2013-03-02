#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "actor/actor.h"

void new_rehearse(Actor* actor);
void new_script(Actor* actor);

// With this model, you can define your own struct
// However, try to avoid having variable sized data
// as the initialisation of an actor doesn't take
// this into account
typedef struct {
  int one;
  double two;
} My_Prop;

int main()
{
  setbuf(stdout, NULL);
  Actor *root_actor = initialise_model();
  root_actor->script = new_script;
  root_actor->props = malloc(sizeof(My_Prop));
  new_rehearse(root_actor);
  actor_spawn(root_actor, new_rehearse, new_script, sizeof(My_Prop));
  read_script(root_actor); 
  finalise_model();
  return 0;
}

void new_rehearse(Actor* actor)
{
  printf("Actor(%d) initialising...\n", actor->id);
  My_Prop* my_prop = actor->props;
  my_prop->one = 0;
  my_prop->two = 2.0;
}

void new_script(Actor* actor)
{
  My_Prop* my_prop = actor->props;
  printf("Actor(%d) has a countdown value of %d\n", actor->id, my_prop->one);
  if(10-my_prop->one > 0)
  {
    my_prop->one++;
  }
  else
  {
    actor->stop = 1;
  }
}
