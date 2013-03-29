#include "../headers/timers.h"

extern int max_time;
extern double year_length;
extern int initial_diseased_frog_count;

Role timer_role = {timer_initialisation, timer_script, timer_encore, sizeof(Timer)};

// Function which initialises the timing actor
void timer_initialisation(Actor* actor){
	int i,j;
  Timer *t_props = actor->props;

  t_props->frog_count = initial_frog_count;
  t_props->diseased_frog_count = initial_diseased_frog_count;
  t_props->year_length = year_length;
	t_props->current_year = 0;
	t_props->waiting = 0;
	t_props->cell_stats = (int*) malloc(2*cell_count*sizeof(int));
	for(i=0; i<2*cell_count; i++){
		t_props->cell_stats[i] = 0;
	}
	actor->act_number=OFF_STAGE;
}

// The job that the Timing actor runs; when the year is over it prints
// information about the state of the system, if the run time is up, it 
// messages the lead_actors of all systems to
void timer_script(Actor* actor){
  int i;
	int *sent_props;
  Timer *t_props = actor->props;
	switch(actor->act_number){
    // if at start, tell everyone to start
		case OFF_STAGE:
			printf("####Blue-Nosed Frog Disease simulation running on %d processor(s)\n", number_of_processes);
			printf("#Year\tTotal Frog Count\tDiseased Frog Count\tLandCell\tPopulationInflux\tInfectionLevel\n");
			for(i=0; i<cell_count;i++){
				printf(
					"%d\t%d\t%d\t%d\t%d\t%d\n",
					t_props->current_year,
					t_props->frog_count,
					t_props->diseased_frog_count,
					i,
					t_props->cell_stats[2*i],
					t_props->cell_stats[2*i+1]
				);
			}
			printf("\n");
			t_props->year_start= MPI_Wtime();
			for(i=1;i<number_of_processes;i++){
				talk(actor, i, OPEN_CURTAINS);
			}
			talk_with_all_proteges(actor, OPEN_CURTAINS);
			actor->act_number = ON_STAGE;
			break;
    // if no messages, check to see if timings have elapsed
		case ON_STAGE:
			if(t_props->waiting == cell_count+1){
				for(i=0;i<cell_count;i++){
					printf(
						"%d\t%d\t%d\t%d\t%d\t%d\n",
						t_props->current_year,
						t_props->frog_count,
						t_props->diseased_frog_count,
						i,
						t_props->cell_stats[2*i],
						t_props->cell_stats[2*i+1]
					);
				}
				printf("\n");
				t_props->waiting = 0;
			} else if(t_props->waiting > 0) {
				return;
			}
			if(t_props->current_year >= max_time){
				printf("#End of simulation\n");
				actor->poison_pill = 1;
			}	else if(MPI_Wtime() - t_props->year_start > t_props->year_length){
				t_props->year_start = MPI_Wtime();
				t_props->current_year++;
				for(i=1;i<=cell_count;i++){
					talk(actor, i, A_MONSOON_BRINGS_IN_THE_NEW_YEAR);
				}
				t_props->waiting = 1;
				if(t_props->frog_count == 0){
					printf("#There are no more frogs left, terminating early\n");
					actor->poison_pill=1;
          return;
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
		case A_LAND_CELL_REMEMBERS_THE_PAST_YEAR:
			sent_props = (int*) actor->sent_props;
			t_props->cell_stats[2*(actor->sender-1)] = sent_props[0];
			t_props->cell_stats[2*(actor->sender-1)+1] = sent_props[1];
			t_props->waiting++;
			actor->act_number = ON_STAGE;
			break;
		case CLOSE_CURTAINS:
			actor->poison_pill = 1;
			break;
	}
}


void timer_encore(Actor *actor){
	Timer *t_props = actor->props;
	free(t_props->cell_stats);
}
