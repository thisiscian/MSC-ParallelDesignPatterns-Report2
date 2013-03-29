#include "../headers/land_cells.h"

Role land_cell_role = {land_cell_initialisation, land_cell_script, land_cell_encore, sizeof(Land_Cell)};

void land_cell_initialisation(Actor* actor){
  Land_Cell *lc_props = actor->props;
  lc_props->population_influx=0;
  lc_props->infection_level=0;
	actor->act_number=OFF_STAGE;
  // create linked list of each land cell
	generate_load_list(actor);
}

void land_cell_script(Actor* actor){
	int my_data[2];
	int *data;
	int cell;
	float *pos;
	Land_Cell* lc_props = actor->props;
	switch (actor->act_number){
    // start when timer says to start
		case OPEN_CURTAINS:
			talk_with_all_proteges(actor, OPEN_CURTAINS);
			actor->act_number = ON_STAGE;
			break;
    // stop when told
		case CLOSE_CURTAINS:
    	actor->poison_pill = 1;
			break;
    // print data on new year
		case A_MONSOON_BRINGS_IN_THE_NEW_YEAR:
	    my_data[0] = lc_props->population_influx;
			my_data[1] = lc_props->infection_level;
			interact(actor, actor->sender, A_LAND_CELL_REMEMBERS_THE_PAST_YEAR, 2, MPI_INT, my_data);
    	lc_props->population_influx = 0;
    	lc_props->infection_level = 0;
			actor->act_number = OFF_STAGE;
			break;
    // give frog land cell data
		case A_FROG_HOPS_INTO_THE_UNKNOWN:
	    my_data[0] = lc_props->population_influx;
			my_data[1] = lc_props->infection_level;
	  	data = (int*) actor->sent_props;
			interact(actor, actor->sender, A_FROG_SURVEYS_THE_LAND, 2, MPI_INT, my_data);
	    lc_props->population_influx++;
	    lc_props->infection_level += *data;
			actor->act_number = OFF_STAGE;
			break;
    // tell frog least loaded processor
		case A_FROG_NEEDS_SPACE_TO_SPAWN:
			cell = find_least_loaded_process(lc_props->load_list);
			interact(actor,actor->sender,A_LAND_CELL_KNOWS_SUCH_A_PLACE, 1, MPI_INT, &cell);
			actor->act_number = OFF_STAGE;
			break;
    // updated load list
		case A_LAND_CELL_CHANGES:
			data = (int*) actor->sent_props;
			set_land_cell_load(actor, actor->sender, *data);
			actor->act_number = OFF_STAGE;
			break;
    // if asked to spawn a new frog, do so, and updated load lists
		case A_LAND_CELL_ADOPTS_A_TADPOLE:
			pos = (float*) actor->sent_props;
			talk(actor,0,A_FROG_SPAWNS);
			actor_train_protege(actor, frog_role, &(pos));
			step_load(actor, +1);
			message_load_to_all(actor);
			actor->act_number = OFF_STAGE;
			break;
	}
}

void generate_load_list(Actor* actor){
	int i;
	Land_Cell *lc_props = actor->props;
	lc_props->load_list = make_new_load_list(1,0);
	Load_List *list = lc_props->load_list;
	for(i=2;i<=cell_count;i++){
		list->next = make_new_load_list(i, 0);
		list=list->next;
	}
}

void set_land_cell_load(Actor *actor, int id, int load){
	Land_Cell *lc_props = actor->props;
	Load_List *list = lc_props->load_list;

	while(list->id != id){
		list = list->next;
	}
	list->load = load;
	return;
}

void step_load(Actor *actor, int mod){
	Land_Cell *lc_props = actor->props;
	Load_List *list = lc_props->load_list;

	while(list->next != NULL && list->id != actor->id){
		list = list->next;
	}
	list->load += mod;
	return;
}

void message_load_to_all(Actor *actor){
	int load;
	Land_Cell *lc_props = actor->props;
	Load_List *list = lc_props->load_list;

	while(list->id != actor->id){
		list = list->next;
	}
	load = list->load ;
	list = lc_props->load_list;
	while(list != NULL){
		if(list->id != actor->id){
			interact(actor, list->id, A_LAND_CELL_CHANGES, 1, MPI_INT, &load);
		}
		list = list->next;
	}
	return;
}

Load_List* make_new_load_list(int id, int load){
	Load_List *list = malloc(sizeof(Load_List));

	list->id = id;
	list->load = load;
	list->next = NULL;
	return list;
}

// finds the least loaded processor, and then returns the least loaded cell on that processor
int find_least_loaded_process(Load_List *load_list){
	int i;
	int proc_load[number_of_processes];
	int min_on_proc[number_of_processes][2]; //array of land cell id and corresponding process id
	int min=0, rank;
	Load_List *list = load_list;

	initialise_array(proc_load, number_of_processes, 0);
	for(i=0;i<number_of_processes;i++)
	{
		min_on_proc[i][0] = -1;
		min_on_proc[i][1] = -1;
	}
	while(list != NULL){
		rank=list->id%number_of_processes;
		proc_load[rank] += list->load;
		if(min_on_proc[rank][1] > list->load){
			min_on_proc[rank][0] = list->id;
			min_on_proc[rank][1] = list->load;
		} else if(min_on_proc[rank][1] == -1){
			min_on_proc[rank][0] = list->id;
			min_on_proc[rank][1] = list->load;
    }
		list = list->next;	
	}
  
	for(i=1;i<number_of_processes;i++){
		min = proc_load[min] < proc_load[i] ? min : i;
	}
	return min_on_proc[min][0];	
}

// frees the load list
void land_cell_encore(Actor *actor){
  Land_Cell *lc_props = actor->props;
  Load_List *tmp;
  Load_List *list = lc_props->load_list;
  while(list != NULL){
    tmp = list;
    list = list->next;
    free(tmp);
  }
  return;
}
