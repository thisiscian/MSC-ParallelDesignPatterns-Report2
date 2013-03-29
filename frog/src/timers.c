#include "../headers/timers.h"

extern int max_time;
extern double year_length;
extern int initial_diseased_frog_count;

Role timer_role = {timer_initialisation, timer_script, no_encore, sizeof(Timer)};

// Function which initialises the timing actor
void timer_initialisation(Actor* actor){
  Timer *t_props = actor->props;
  t_props->frog_count = initial_frog_count;
  t_props->diseased_frog_count = initial_diseased_frog_count;
  t_props->year_length = year_length;
	t_props->current_year = 0;

	actor->act_number=OFF_STAGE;
}

// The job that the Timing actor runs; when the year is over it prints
// information about the state of the system, if the run time is up, it 
// messages the lead_actors of all systems to
void timer_script(Actor* actor){
  int i;
  Timer *t_props = actor->props;
	switch(actor->act_number){
    // if at start, tell everyone to start
		case OFF_STAGE:
			printf("####Blue-Nosed Frog Disease simulation running on %d processor(s)\n", number_of_processes);
			printf("#Year %d of %d\tTotal Frog Count = %d\tDiseased Frog Count = %d\n",t_props->current_year, max_time, t_props->frog_count, t_props->diseased_frog_count);
			printf("#LandCell\tPopulationInflux\tInfectionLevel\n");
			t_props->year_start= MPI_Wtime();
			for(i=1;i<number_of_processes;i++){
				talk(actor, i, OPEN_CURTAINS);
			}
			talk_with_all_proteges(actor, OPEN_CURTAINS);
			actor->act_number = ON_STAGE;
			break;
    // if no messages, check to see if timings have elapsed
		case ON_STAGE:
			if(t_props->current_year >= max_time){
				printf("#End of simulation\n");
				actor->poison_pill = 1;
			}	else if(MPI_Wtime() - t_props->year_start > t_props->year_length){
				if(t_props->frog_count == 0){
					printf("#There are no more frogs left, terminating early\n");
					actor->poison_pill=1;
          return;
				}
				t_props->year_start = MPI_Wtime();
				t_props->current_year++;
			  printf("\n#Year %d of %d\tTotal Frog Count = %d\tDiseased Frog Count = %d\n",t_props->current_year, max_time, t_props->frog_count, t_props->diseased_frog_count);
			  printf("#LandCell\tPopulationInflux\tInfectionLevel\n");
				for(i=1;i<=cell_count;i++){
					talk(actor, i, A_MONSOON_BRINGS_IN_THE_NEW_YEAR);
				}
			}
			break;
    // increase frog count, quit if too high
		case A_FROG_SPAWNS:
			t_props->frog_count++;
		  if(t_props->frog_count > max_frog_count){
				printf("#ERROR: frog count exceeded maximum (%d), exiting... \n", max_frog_count);
				actor->poison_pill = 1;
			}
      actor->act_number = ON_STAGE;
			break;
    // increment disease counter
		case A_FROG_CONTRACTS_THE_PLAGUE:
			t_props->diseased_frog_count++;
			actor->act_number = ON_STAGE;
			break;
    // if a frog dies, decrement the diseased and total frog counts
		case A_FROG_CROAKS:
			t_props->frog_count--;
			t_props->diseased_frog_count--;
			actor->act_number = ON_STAGE;
			break;
		case CLOSE_CURTAINS:
			actor->poison_pill = 1;
			break;
	}
}

