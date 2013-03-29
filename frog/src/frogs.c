#include "../headers/frogs.h"

Role frog_role = {frog_initialisation, frog_script, no_encore, sizeof(Frog)};

// give some frogs disease
int choose_disease(Actor* actor){
	return (actor->id <= cell_count+initial_diseased_frog_count);
}

// initialise frogs
void frog_initialisation(Actor* actor, void *prop){
  float *props;
	int *year;
  Frog *f_props = actor->props;

  f_props->hop_count=0;
  f_props->hops_this_year=0;
  f_props->diseased=choose_disease(actor);
	initialise_array(f_props->population_history, 300, 0);
	initialise_array(f_props->infection_history, 300, 0);
	f_props->state = -1-(actor->id);
 	initialiseRNG(&(f_props->state)); // each frog has a random variable
	if(prop == NULL) {
    // if initial frog, start from pos=0,0, and do not start working immediately
		actor->act_number = OFF_STAGE;
  	frogHop(0, 0, &(f_props->pos[0]), &(f_props->pos[1]), &(f_props->state));
	f_props->current_year = 0;
	} else {
    // if new frog, start from predecessors position
		props = (float*) prop;	
		year = (int *)&(props[2]);
		f_props->pos[0] = props[0];
		f_props->pos[1] = props[1];
		f_props->current_year = *year;
		actor->act_number = ON_STAGE;
	}
}

void frog_script(Actor* actor){
  Frog *f_props = actor->props;
	int i,*sent_props, new_cell;
	void *new_frog_props;
	float *props;
	int *year;
	float average_cell_population = 0;
	float average_cell_infection = 0;
	switch(actor->act_number){
    // received start message from timer actor
		case OPEN_CURTAINS:
			talk_with_all_proteges(actor, OPEN_CURTAINS);
			actor->act_number = ON_STAGE;
			break;
    // first stage of normal frog work, ends when waiting for data
		case ON_STAGE:
	  	frogHop(f_props->pos[0], f_props->pos[1], &(f_props->pos[0]), &(f_props->pos[1]), &(f_props->state));
	  	f_props->hop_count++;
	  	f_props->hops_this_year++;
      // if years are counted in hops, and frog is over hop limit, start monsoon
      if(year_type == 1 && f_props->hops_this_year >= hop_limit){
        for(i=0;i<number_of_processes;i++){
					interact(actor, i, A_MONSOON_BRINGS_IN_THE_NEW_YEAR, 1, MPI_INT, &(f_props->current_year));
				}
				f_props->hops_this_year = 0;
      }
			f_props->current_cell = getCellFromPosition(f_props->pos[0], f_props->pos[1])%cell_count;
      // tell land cell that you have hopped into it, and tell it your disease status
			interact(actor,f_props->current_cell+1,A_FROG_HOPS_INTO_THE_UNKNOWN, 1, MPI_INT, &(f_props->diseased));
			actor->act_number = OFF_STAGE; // wait off stage for response
			break;
    // received land cell data
		case A_FROG_SURVEYS_THE_LAND:
			sent_props = (int*) actor->sent_props;
      // add data to histories
			f_props->population_history[f_props->hop_count%300] += sent_props[0];
			f_props->infection_history[f_props->hop_count%500] += sent_props[1];

      // check to see if frog gets diseased
			if(!f_props->diseased){
				calculate_average(f_props->infection_history, 500, &average_cell_infection);
				if(willCatchDisease(average_cell_infection, &(f_props->state))){
					talk(actor,0, A_FROG_CONTRACTS_THE_PLAGUE);
					f_props->diseased=1; 
				}
			}
      // check to see if diseased frog dies
			if(f_props->hop_count%700  == 0 && f_props->diseased && willDie(&(f_props->state))){
				talk(actor,0, A_FROG_CROAKS);
				actor->retire = 1;
			}
      // check to see if frog gives birth
			if(f_props->hop_count % 300 == 0){	
				calculate_average(f_props->population_history, 300, &average_cell_population);
				if(willGiveBirth(average_cell_population, &(f_props->state))){
          // ask current land cell for least loaded processor
					talk(actor, f_props->current_cell+1, A_FROG_NEEDS_SPACE_TO_SPAWN);
					actor->act_number = OFF_STAGE;
					break;
				}
			}
			actor->act_number = ON_STAGE;
			break;
    // receive information on least loaded processor
		case A_LAND_CELL_KNOWS_SUCH_A_PLACE:
			new_cell = *((int*) actor->sent_props);
			new_frog_props = malloc(2*sizeof(float)+sizeof(int));
			props = (float*) new_frog_props;
			year = (int*) &(props[2]);
			props[0] = f_props->pos[0];
			props[1] = f_props->pos[1];
			*year = f_props->current_year;
      // tell that processor to spawn a new frog with current position
			interact(actor, new_cell, A_LAND_CELL_ADOPTS_A_TADPOLE, 2*sizeof(float)+sizeof(int), MPI_BYTE, new_frog_props);
			actor->act_number = ON_STAGE;
			break;
		// pass the monsoon onto proteges, reset hop counter
    case A_MONSOON_BRINGS_IN_THE_NEW_YEAR:
			sent_props = (int*) actor->sent_props;
			if(sent_props != NULL && f_props->current_year == *sent_props){
	      interact_with_all_proteges(actor, A_MONSOON_BRINGS_IN_THE_NEW_YEAR, 1, MPI_INT, sent_props);
				f_props->current_year++;
    	  f_props->hops_this_year = 0;
			}
      actor->act_number = ON_STAGE;
      break;
	}
}
