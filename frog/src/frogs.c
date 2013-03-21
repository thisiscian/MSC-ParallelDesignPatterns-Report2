#include "../headers/frogs.h"

Role frog_role = {frog_initialisation, frog_script, sizeof(Frog)};

int choose_disease(Actor* actor){
	return actor->id < 10;
}

void frog_initialisation(Actor* actor){
  int i;
  Frog *f_props = actor->props;

  f_props->hop_count=0;
  f_props->diseased=choose_disease(actor);
	initialise_array(f_props->population_history, 300, 0);
	initialise_array(f_props->infection_history, 300, 0);

  f_props->state = -1-(actor->id);
  initialiseRNG(&(f_props->state));
	
	if(actor->mentor == NULL){
		actor->act_number = OFF_STAGE;
  	frogHop(0, 0, &(f_props->x), &(f_props->y), &(f_props->state));
	} else {
		Frog *mentor_props = actor->mentor->props;
		f_props->x = mentor_props->x;
		f_props->y = mentor_props->y;
		actor->act_number = OPEN_CURTAINS;
	}
}

void frog_script(Actor* actor){
  Frog *f_props = actor->props;
	int i;
	int *cell_stats;
	float average_cell_population = 0;
	float average_cell_infection = 0;
	if(actor->act_number == OPEN_CURTAINS){
		actor->act_number = ON_STAGE;
		talk_with_all_proteges(actor, OPEN_CURTAINS);
	} else if(actor->act_number == CLOSE_CURTAINS){
    actor->poison_pill = 1;
  }	else if(actor->act_number == ON_STAGE) {
  	frogHop(f_props->x, f_props->y, &(f_props->x), &(f_props->y), &(f_props->state));
  	f_props->hop_count++;
		f_props->current_cell = getCellFromPosition(f_props->x, f_props->y)%initial_cell_count;
		if(f_props->current_cell < 0)
		{
			printf("Frog(%d): Cell less than 0\n\tstate=%d\n\tpos=(%f,%f)\n", actor->id, f_props->state, f_props->x,f_props->y);
		}
		interact(actor,f_props->current_cell+1,A_FROG_HOPS_INTO_THE_UNKNOWN, 1, MPI_INT, &(f_props->diseased));
		actor->act_number = OFF_STAGE;
	} else if(actor->act_number ==  A_FROG_SURVEYS_THE_LAND){
		cell_stats = (int*) actor->sent_props;
  	f_props->population_history[f_props->hop_count%300] += cell_stats[0];
  	f_props->infection_history[f_props->hop_count%500] += cell_stats[1];
		if(f_props->hop_count % 300 == 0){	
		  for(i=0; i<300; i++){
		    average_cell_population += f_props->population_history[i];
		  }
	  	average_cell_population/=300;
			if(willGiveBirth(average_cell_population, &(f_props->state))){
				talk(actor,0, A_FROG_SPAWNS);
				Actor* baby_frog;
				if(actor->mentor != NULL){
					baby_frog = actor_train_protege(actor->mentor, frog_role);
				} else {
					baby_frog = actor_train_protege(actor, frog_role);
				}
			}
		}
		if(f_props->hop_count % 500 == 0 && !f_props->diseased){
		  for(i=0; i<500; i++){
		    average_cell_infection += f_props->infection_history[i];
		  }
		  average_cell_infection/=500;
	
			if(willCatchDisease(average_cell_infection, &(f_props->state))){
				printf("am sick\n");
				talk(actor,0, A_FROG_CONTRACTS_THE_PLAGUE);
				talk(actor,f_props->current_cell+1, A_FROG_CONTRACTS_THE_PLAGUE);
				f_props->diseased=1; 
			}
		}
		if(f_props->hop_count%700  == 0 && f_props->diseased && willDie(&(f_props->state))){
			talk(actor,0, A_FROG_CROAKS);
  	  actor->retire = 1;
  	}
		actor->act_number = ON_STAGE;
	}
}
