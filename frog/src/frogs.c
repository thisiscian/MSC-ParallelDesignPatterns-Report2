#include "../headers/frogs.h"

Role frog_role = {frog_initialisation, frog_script, sizeof(Frog)};

void frog_initialisation(Actor* actor)
{
  int i;
  Frog *f_props = actor->props;

  frogHop(0, 0, &(f_props->x), &(f_props->y), f_props->state);
  f_props->hop_count=0;
  f_props->diseased=0;
  f_props->state = get_seed(actor);
  for(i=0; i<300; i++)
  {
    f_props->population_history[i] = 0;
  }
}

void frog_script(Actor* actor)
{
  float new_x, new_y;
  int i;
  int current_cell;
  int cell_stats[2]; 
  float average_cell_population = 0, average_cell_infection = 0;
  Frog *f_props = actor->props;

  respond_to_dialogue(actor, frog_reaction);

  frogHop(f_props->x, f_props->y, &new_x, &new_y, f_props->state);
  f_props->x = new_x;
  f_props->y = new_y;
  f_props->hop_count++;
  current_cell = getCellFromPosition(f_props->x, f_props->y);
  //### MESSAGE CURRENT CELL TO GET VALUES
  for(i=0; i<300; i++)
  {
    average_cell_population += f_props->population_history[i];
  }
  average_cell_population/=min(700,f_props->hop_count);
  for(i=0; i<700; i++)
  {
    average_cell_infection += f_props->infection_history[i];
  }
  average_cell_infection/=min(700,f_props->hop_count);
  if(willGiveBirth(average_cell_population, f_props->state))
  {
    Actor* baby_frog = actor_train_protege(actor->mentor, frog_initialisation, frog_script, sizeof(Frog));
    Frog* baby_f_props = baby_frog->props;
    baby_f_props->x = f_props->x;
    baby_f_props->y = f_props->y;
  };
  if(willCatchDisease(average_cell_infection, f_props->state))
  {
    f_props->diseased=1; 
  };
}

void frog_reaction(Actor* actor, int message_type, int next_message_size)
{
  if(message_type == RETIRE)
  {
    enter_dialogue_with_all_proteges(actor, RETIRE, 0);
    actor->retire = 1;
  }
}

