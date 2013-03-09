#include <actormetaphor.h>
#include <stdio.h>

Role choose_role(int id);
void reaction(Actor* actor, int message_type, int next_message_size);
void new_rehearse_type_1(Actor* actor);
void new_rehearse_type_2(Actor* actor);
void new_script_type_1(Actor* actor);
void new_script_type_2(Actor* actor);
int memory_allocation(int id);

// With this model, you can define your own struct
// However, try to avoid having variable sized data
// as the initialisation of an actor doesn't take
// this into account.
typedef struct {
  int one;
  float two;
} My_Prop;

Role type_1 = {new_rehearse_type_1, new_script_type_1, sizeof(My_Prop)};
Role type_2 = {new_rehearse_type_2, new_script_type_2, sizeof(My_Prop)};

int main()
{
  setbuf(stdout, NULL);
  Actor *root_actor = actor_initialise_metaphor(choose_role);
  //actor_train_protege(root_actor, new_rehearse, new_script_type_1, sizeof(My_Prop));
  perform(root_actor); 
  actor_finalise_metaphor();
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
  respond_to_dialogue(actor, reaction);
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
  enter_dialogue(actor, 1, 1, 1);
  give_props(actor, 1, 1, MPI_FLOAT, &prop);
  printf("Actor(%d) read line and retiring...\n", actor->id);
  actor->retire = 1;
}

Role choose_role(int id)
{
  if(id == 0)
  {
    return type_2;
  }
  else
  {
    return type_1;
  }
}

void reaction(Actor* actor, int message_type, int next_message_size)
{
  My_Prop* my_prop = actor->props;
  if(message_type == 0)
  {
    printf("Actor(%d) reacting!\n", actor->id);
    actor->retire = 1;
  }
  if(message_type == 1)
  {
    printf("Actor(%d) old value of two: %f\n", actor->id, my_prop->two);
    get_props(actor, next_message_size, MPI_FLOAT, &(my_prop->two));
    printf("Actor(%d) new value of two: %f\n", actor->id, my_prop->two);
  }
}
