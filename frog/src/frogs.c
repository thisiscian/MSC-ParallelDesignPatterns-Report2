#include "../headers/frogs.h"

Role frog_role = {frog_initialisation, frog_script, sizeof(Frog)};

void frog_initialisation(Actor* actor)
{
  int i;
  Frog *f_props = actor->props;
<<<<<<< HEAD
	initialiseRNG(&(f_props->state));
  //f_props->state = get_seed(actor);
  frogHop(0, 0, &(f_props->x), &(f_props->y), &(f_props->state));
=======
  get_seed(actor);
  frogHop(0, 0, &(f_props->x), &(f_props->y), &state);
>>>>>>> 35bef11f6d0b0978af407f375cd6a28c5907edff
	if(actor->id%2 == 0)
	{
  	f_props->diseased=0;
	}
	else
	{
  	f_props->diseased=1;
		enter_dialogue(actor, 0, A_FROG_CONTRACTS_THE_PLAGUE);
		enter_dialogue(actor, f_props->current_cell+1, A_FROG_CONTRACTS_THE_PLAGUE);
	}
  f_props->hop_count=0;
	f_props->current_cell = (getCellFromPosition(f_props->x, f_props->y)%initial_cell_count+initial_cell_count)%initial_cell_count;

  for(i=0; i<300; i++)
  {
    f_props->population_history[i] = 0;
  }
	for(i=0; i<500; i++)
  {
    f_props->infection_history[i] = 0;
  }
	actor->act_number = OPEN_CURTAINS;
}

void frog_script(Actor* actor)
{
  Frog *f_props = actor->props;
  if(actor->act_number == CLOSE_CURTAINS)
  {
    actor->poison_pill = 1;
  }
	else if(actor->act_number == OPEN_CURTAINS)
	{
  	int my_stats[2] = {actor->id, f_props->diseased};
<<<<<<< HEAD
  	frogHop(f_props->x, f_props->y, &(f_props->x), &(f_props->y), &(f_props->state));
=======
		long* A = &state;
  	frogHop(f_props->x, f_props->y, &(f_props->x), &(f_props->y), &state);
>>>>>>> 35bef11f6d0b0978af407f375cd6a28c5907edff
  	f_props->hop_count++;
		f_props->current_cell = (getCellFromPosition(f_props->x, f_props->y)%initial_cell_count+initial_cell_count)%initial_cell_count;
  	enter_dialogue(actor, f_props->current_cell+1,A_FROG_HOPS_INTO_THE_UNKNOWN);
  	give_props(actor, f_props->current_cell+1, 2, MPI_INT, my_stats);
		actor->act_number = OFF_STAGE;
	}
	else if(actor->act_number ==  A_FROG_SURVEYS_THE_LAND)
	{
  	int i, cell_stats[2]; 
	  get_props(actor, 2, MPI_INT, cell_stats);
  	f_props->population_history[f_props->hop_count%300] += cell_stats[0];
  	f_props->infection_history[f_props->hop_count%700] += cell_stats[1];
		if(f_props->hop_count % 300 == 0)
		{	
			float average_cell_population = 0;
		  for(i=0; i<300; i++)
		  {
		    average_cell_population += f_props->population_history[i];
		  }
	  	average_cell_population/=300;
			if(willGiveBirth(average_cell_population, &state))
			{
				printf("Frog(%d) is giving birth\n", actor->id);
				enter_dialogue(actor, 0, A_FROG_SPAWNS);
				Actor* baby_frog;
				if(actor->mentor != NULL) 
				{
					baby_frog = actor_train_protege(actor->mentor, frog_role);
				}
				else
				{
					baby_frog = actor_train_protege(actor, frog_role);
				}
				Frog* baby_f_props = baby_frog->props;
				baby_f_props->x = f_props->x;
				baby_f_props->y = f_props->y;
				baby_frog->rehearse(baby_frog);
			}
		}
		if(f_props->hop_count % 500 == 0)
		{
  		float average_cell_infection = 0;
		  for(i=0; i<500; i++)
		  {
		    average_cell_infection += f_props->infection_history[i];
		  }
		  average_cell_infection/=500;
	
			if(willCatchDisease(average_cell_infection, &state) && !f_props->diseased)
			{
				printf("Frog(%d) is sick\n", actor->id);
				enter_dialogue(actor, 0, A_FROG_CONTRACTS_THE_PLAGUE);
				enter_dialogue(actor, f_props->current_cell+1, A_FROG_CONTRACTS_THE_PLAGUE);
				f_props->diseased=1; 
			}
		}
		if(f_props->hop_count%700  == 0 && f_props->diseased && willDie(&state))
		{
  	  enter_dialogue(actor, 0, A_FROG_CROAKS);
  	  actor->retire = 1;
  	}
		actor->act_number = OPEN_CURTAINS;
	}
}
