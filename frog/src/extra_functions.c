#include "../headers/extra_functions.h"

// Standard min function definition
int min(int a, int b){
  return a<=b?a:b;
}

Role choose_role(int id){
  if(id == 0){
    return timer_role;
  } else if(id > 0 && id <= cell_count){
    return land_cell_role;
  } else if(id > cell_count && id <= cell_count+initial_frog_count){
    return frog_role;
  } else {
    return NULL_ROLE;
  }
}

void initialise_array(int *arr, int size, int val){
	int i;
	for(i=0; i<size;i++){
		arr[i] = val;
	}
	return;
}
	
int collect_input(int argc, char *argv[]){
	int i=1, err=0;

	// default settings
	initial_frog_count = 34;
	initial_diseased_frog_count = 17;
 	cell_count = 16;
 	max_frog_count = 100;
 	max_time = 100;
	year_length = 1;
	buff_size = pow(2,12)*sizeof(double);

	while(i<argc && err == 0)
	{
		if(!strcmp(argv[i], "--frog")) {
			if(i+1 < argc){
				initial_frog_count = atoi(argv[i+1]);
				i+=2;
			}	else {
				printf("Error: flag '--frog' requires that an integer follows it\n");
				err++;
			}
		} else if(!strcmp(argv[i], "--disease")) {
			if(i+1 < argc){
				initial_diseased_frog_count = atoi(argv[i+1]);
				i+=2;
			}	else {
				printf("Error: flag '--disease' requires that an integer follows it\n");
				err++;
			}
		} else if(!strcmp(argv[i], "--cell")) {
			if(i+1 < argc){
				cell_count = atoi(argv[i+1]);
				i+=2;
			} else {
				printf("Error: flag '--cell' requires that an integer follows it\n");
				err++;
			}
		} else if(!strcmp(argv[i], "--max-frog")) {
			if(i+1 < argc){
				cell_count = atoi(argv[i+1]);
				i+=2;
			} else {
				printf("Error: flag '--max-frog' requires that an integer follows it\n");
				err++;
			}
		} else if(!strcmp(argv[i], "--max-year")) {
			if(i+1 < argc){
				max_time = atoi(argv[i+1]);
				i+=2;
			} else {
				printf("Error: flag '--max-year' requires that an integer follows it\n");
				err++;
			}
		} else if(!strcmp(argv[i], "--year-length")) {
			if(i+1 < argc){
				year_length = atof(argv[i+1]);
				i+=2;
			} else {
				printf("Error: flag '--year-length' requires that a number follows it\n");
				err++;
			}
		} else if(!strcmp(argv[i], "--buffer-size")) {
			if(i+1 < argc){
				buff_size = atof(argv[i+1])*1024;
				i+=2;
			} else {
				printf("Error: flag '--buffer_size' requires that a number follows it\n");
				err++;
			}
		} else if(!strcmp(argv[i], "--help")) {
			help(argv[0]);
			err++;
		} else {
			printf("Error: unknown flag %s\n", argv[i]);
			err++;
		}
	}	
	if(initial_frog_count < initial_diseased_frog_count){
		printf("Error: There are more diseased frogs (%d) than total number of frogs (%d)\n", initial_frog_count, initial_diseased_frog_count);
		printf("       Try adding the flag '--disease %d' to have all frogs start diseased\n", initial_frog_count);
		err++;
	}
	return err;
}

void help(char* program_name)
{
	printf("Usage: %s [options] \n", program_name);
	printf("Options:\n");
	printf("  --help\t\tshow this help message\n");
	printf("  --frog n\t\tpopulates the model with 'n' frogs. (default 34) \n");
	printf("  --disease n\t\tensures that 'n' of the frogs start off diseased. (default 17)\n");
	printf("  --max-frog n\t\tthe maximum number of frogs that can be spawned before the program exits. (default 100) \n");
	printf("  --cell n\t\tthe number of land cells that the frogs can move on. (default 16)\n");
	printf("\t\t\t\tnote: some functions presume there are 16 land cells.\n");
	printf("\t\t\t\tUsing a land cell count that is not 16 will lead to erratic frog movements.\n");
	printf("  --max-year n\t\tthe number of years that the simulation runs for. (default 100)\n");
	printf("  --year-length m\tdefines the length of a year in (wall clock) seconds. Can be a real number. (default 1.0) \n");
	printf("  --buffer-size m\tdefines the MPI Buffer size in Mb. Can be a real number. (default 32.0)\n");
	return;
}
