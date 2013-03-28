/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

#include "../header/am_misc.h"
#include "../header/am_role.h"

int next_id = 0;
int process_rank;
int number_of_processes;
int buff_size=1000*sizeof(double);
void *buf;

Actor* actor_initialise_metaphor (Role (*choose_role)(int id)){
 	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);
  buf = malloc(buff_size);
	MPI_Buffer_attach(buf,buff_size);
	int id = get_next_id();
	Actor *actor;
	
  if(choose_role == NULL){
    actor = _train_actor(NULL, id, NULL_ROLE, NULL);
  } else {
    actor = _train_actor(NULL, id, choose_role(id), NULL);
  }
	return actor;
}

// Receive any leftover messages and then finalise MPI
void actor_finalise_metaphor(Actor *actor){
	int flag;
	int count;
	MPI_Status status;
	MPI_Iprobe(process_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
	while(flag){
		MPI_Get_count(&status,MPI_INT, &count);
		int var[count];
		MPI_Recv(&var,count,MPI_INT,process_rank,MPI_ANY_TAG,MPI_COMM_WORLD, &status);
		MPI_Iprobe(process_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
	}
  _retire_actor(actor);
	MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
  free(buf);
}

int peek_next_id(){
	return process_rank+next_id*number_of_processes; 
}

int get_next_id(){
  next_id++;
	return process_rank+(next_id-1)*number_of_processes; 
}

void no_rehearse(Actor *actor){}
void no_script(Actor *actor){}
void no_encore(Actor *actor){}
