#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "actor/actor.h"

void reaction(Actor* actor, int message_type, int next_message_size);
void new_rehearse_type_1(Actor* actor);
void new_rehearse_type_2(Actor* actor);
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
  float two;
} My_Prop;

int main()
{
  setbuf(stdout, NULL);
  Actor *root_actor = initialise_actor_model(choose_rehearse, choose_script, memory_allocation);
  //actor_spawn(root_actor, new_rehearse, new_script_type_1, sizeof(My_Prop));
  read_script(root_actor); 
  finalise_actor_model();
  return 0;
}

void new_rehearse_type_1(Actor* actor)
{
  printf("Actor(%d) initialising...\n", actor->id);
  My_Prop* my_prop = actor->props;
  my_prop->one = 0;
  my_prop->two = 0.0;
}

void new_rehearse_type_2(Actor* actor)
{
  printf("Actor(%d) initialising...\n", actor->id);
  My_Prop* my_prop = actor->props;
  my_prop->one = 1;
  my_prop->two = 2.0;
}

void new_script_type_1(Actor* actor)
{
  My_Prop* my_prop = actor->props;
  printf("Actor(%d) waiting for line\n", actor->id); 
  react_to_line(actor, reaction);
  my_prop->one++;
  if(my_prop->one > 10)
  {
    actor->retire = 1;
  }
}

void new_script_type_2(Actor* actor)
{
  My_Prop* my_prop = actor->props;
  float prop = my_prop->two;
  printf("Actor(%d) telling proc 1 I have data to send (data = %f)\n", actor->id, prop);
  read_line(actor, 1, 1, 1);
  give_props(actor, 1, 1, MPI_FLOAT, &prop);
  printf("Actor(%d) read line and retiring...\n", actor->id);
  actor->retire = 1;
}

void (*choose_rehearse(int id))(Actor* actor)
{
  if(id == 0)
  {
    return new_rehearse_type_2;    
  }
  else
  {
    return new_rehearse_type_1;
  }
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

void reaction(Actor* actor, int message_type, int next_message_size)
{
  My_Prop* my_prop = actor->props;
  float props = 25.3;
  if(message_type == 0)
  {
    printf("Actor(%d) reacting!\n", actor->id);
    actor->retire = 1;
  }
  if(message_type == 1)
  {
    printf("Actor(%d) old value of two: %f\n", actor->id, props);
    get_props(actor, next_message_size, MPI_FLOAT, &props);
    printf("Actor(%d) new value of two: %f\n", actor->id, props);
  }
}
