#include "../headers/extra_functions.h"

// Standard min function definition
int min(int a, int b)
{
  return a<=b?a:b;
}

Role choose_role(int id)
{
  if(id == 0)
  {
    return timer_role;
  }
  else if(id > 0 && id <= initial_cell_count)
  {
    return land_cell_role;
  }
  else if(id > initial_cell_count && id <= initial_cell_count+initial_frog_count)
  {
    return frog_role;
  }
  else
  {
    return NULL_ROLE;
  }
}

void initialise_array(int *arr, int size, int val)
{
	int i;
	for(i=0; i<size;i++)
	{
		arr[i] = val;
	}
	return;
}
