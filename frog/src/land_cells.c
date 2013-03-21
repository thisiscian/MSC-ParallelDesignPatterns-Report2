#include "../headers/land_cells.h"

Role land_cell_role = {land_cell_initialisation, land_cell_script, sizeof(Land_Cell)};

void land_cell_initialisation(Actor* actor)
{
  Land_Cell *lc_props = actor->props;
  lc_props->population_influx=0;
  lc_props->infection_level=0;
	actor->act_number=OFF_STAGE;
}

void land_cell_script(Actor* actor)
{
	Land_Cell* lc_props = actor->props;
	if(actor->act_number == OPEN_CURTAINS)
	{
		talk_with_all_proteges(actor, OPEN_CURTAINS);
		actor->act_number = ON_STAGE;
	}
	else if(actor->act_number == CLOSE_CURTAINS)
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
    int frog_data = *((int *)actor->sent_props);
		interact(actor, actor->sender, A_FROG_SURVEYS_THE_LAND, 2, MPI_INT, my_data);
    lc_props->population_influx++;
    lc_props->infection_level += frog_data;
		actor->act_number = OFF_STAGE;
  }
}
