#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "actor/actor.h"

void new_rehearse(Actor* actor);
void new_script_type_1(Actor* actor);
void new_script_type_2(Actor* actor);
void (*choose_rehearse(int id))(Actor* actor);
void (*choose_script(int id))(Actor* actor);
int memory_allocation(int id);

// With this model, you can define your own struct
// However, try to avoid having variable sized data
// as the initialisation of an actor doesn't take
// this into account.
typedef struct {
  int one;
  double two;
} My_Prop;

int main()
{
  setbuf(stdout, NULL);
  Actor *root_actor = initialise_actor_model(choose_rehearse, choose_script, memory_allocation);
  actor_spawn(root_actor, new_rehearse, new_script_type_1, sizeof(My_Prop));
  read_script(root_actor); 
  finalise_actor_model();
  return 0;
}

void new_rehearse(Actor* actor)
{
  printf("Actor(%d) initialising...\n", actor->id);
  My_Prop* my_prop = actor->props;
  my_prop->one = actor->id;
  my_prop->two = 2.0;
}

void new_script_type_1(Actor* actor)
{
  My_Prop* my_prop = actor->props;
  printf("Actor(%d) (type 1) has a countdown value of %d\n", actor->id, 10-my_prop->one);
  if(actor->id < actor->mpi_size || 10-my_prop->one <= 0)
  {
    printf("Actor(%d) is retiring\n", actor->id);
    actor->retire = 1;
  }
  else
  {
    my_prop->one++;
  }
}

void new_script_type_2(Actor* actor)
{
  My_Prop* my_prop = actor->props;
  printf("Actor(%d) (type 2) has a countdown value of %d\n", actor->id, 10-my_prop->one);
  if(10-my_prop->one <= 0)
  {
    printf("Actor(%d) is retiring\n", actor->id);
    actor->retire = 1;
  }
  else
  {
    my_prop->one++;
  }
}

void (*choose_rehearse(int id))(Actor* actor)
{
  return new_rehearse;
}

void (*choose_script(int id))(Actor* actor)
{
  if(id == 0)
  {
    return new_script_type_2;    
  }
  else
  {
    return new_script_type_1;
  }
}

int memory_allocation(int id)
{
  return sizeof(My_Prop);
}

