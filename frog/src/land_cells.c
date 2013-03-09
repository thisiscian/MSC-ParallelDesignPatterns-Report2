#include "../headers/land_cells.h"

Role land_cell_role = {land_cell_initialisation, land_cell_script, sizeof(Land_Cell)};

void land_cell_initialisation(Actor* actor)
{
  Land_Cell *lc_props = actor->props;
  lc_props->land_cell_id = land_cell_id_counter;
  lc_props->state = get_seed(actor);
  land_cell_id_counter++;
  lc_props->population_influx=0;
  lc_props->infection_level=0;
}

void land_cell_script(Actor* actor)
{
  respond_to_dialogue(actor, land_cell_reaction);
}

void land_cell_reaction(Actor* actor, int message_type, int next_message_size)
{
  Land_Cell *lc_props = actor->props;
  if(message_type == MONSOON)
  {
    lc_props->population_influx = 0;
    lc_props->infection_level = 0;
  } 
  else if(message_type == HOP_IN)
  {
    int new_disease_value;
    lc_props->population_influx = 0;
    get_props(actor, next_message_size, MPI_INT, &new_disease_value);
    lc_props->infection_level += new_disease_value;
  }
  else if(message_type == RETIRE)
  {
    enter_dialogue_with_all_proteges(actor, RETIRE, 0);
    actor->retire = 1;
  }
}

