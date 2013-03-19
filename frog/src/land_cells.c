#include "../headers/land_cells.h"

Role land_cell_role = {land_cell_initialisation, land_cell_script, sizeof(Land_Cell)};

void land_cell_initialisation(Actor* actor)
{
  Land_Cell *lc_props = actor->props;
  get_seed(actor);
  lc_props->population_influx=0;
  lc_props->infection_level=0;

	actor->act_number=OPEN_CURTAINS;
}

void land_cell_script(Actor* actor)
{
	Land_Cell* lc_props = actor->props;
	if(actor->act_number == CLOSE_CURTAINS)
	{
    actor->poison_pill = 1;
	}
	else if(actor->act_number == A_MONSOON_BRINGS_IN_THE_NEW_YEAR)
  {
    lc_props->population_influx = 0;
    lc_props->infection_level = 0;
		actor->act_number = OFF_STAGE;
  }
	else if(actor->act_number == A_FROG_HOPS_INTO_THE_UNKNOWN)
  {
    int my_data[2] = {lc_props->population_influx, lc_props->infection_level};
    int frog_data[2];
    get_props(actor, 2, MPI_INT, frog_data);
		enter_dialogue(actor, actor->sender, A_FROG_SURVEYS_THE_LAND);
    give_props(actor, actor->sender, 2, MPI_INT, my_data);
    lc_props->population_influx++;
    lc_props->infection_level += frog_data[1];
		actor->act_number = OFF_STAGE;
  }
}
