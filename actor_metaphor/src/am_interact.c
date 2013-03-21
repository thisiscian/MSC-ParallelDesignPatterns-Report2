/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

#include "../header/am_interact.h"
#include "../header/am_misc.h"
#include "../header/am_proteges.h"
#include <string.h>

// Allows actors to send messages and data to each other via MPI
// - storage is a char* type pointer because chars are 1 byte, which makes it easy to send flexible messages
void interact(Actor *actor, int interact_id, int message, int message_size, MPI_Datatype datatype, void *prop){
	int size = 3*sizeof(int)+message_size*sizeof(datatype);
	int destination_rank = interact_id%number_of_processes;

	void* storage = malloc(size);
	memset(storage, 0, size);
	int *info = (int *) storage;
	info[0] = actor->id;
	info[1] = message;
	info[2] = message_size;	
	if(message_size != 0 && prop != NULL){
		memcpy(info+3, prop, message_size*sizeof(datatype));
	}

	MPI_Bsend(storage, size, MPI_BYTE, destination_rank, interact_id, MPI_COMM_WORLD);
	free(storage);
	return;
}

// Wrapper for `interact` which makes sending only messages easier
void talk(Actor *actor, int interact_id, int message){
	interact(actor, interact_id, message, 0, MPI_BYTE, NULL);
	return;
}

// Allows an actor to send a message-prop pair to all proteges
void interact_with_all_proteges(Actor *actor, int message, int message_size, MPI_Datatype datatype, void *prop){
  Protege* protege;
	protege = actor->proteges;
  while(protege != NULL){
		interact(actor, protege->actor->id, message, message_size, datatype, prop);
    protege = protege->next;
  }
	return;
}

// Wrapper for `interact_with_all_proteges` to simplify only-message sending
void talk_with_all_proteges(Actor *actor, int message){
  Protege* protege;
	protege = actor->proteges;
  while(protege != NULL){
		talk(actor, protege->actor->id, message);
    protege = protege->next;
  }
	return;
}

// Allows actor to receive messages from an `interact` send
void _be_interacted_with(Actor *actor)
{
	int flag, size;
	int *info;
	void *storage;
  MPI_Status status;
	MPI_Iprobe(MPI_ANY_SOURCE, actor->id, MPI_COMM_WORLD, &flag, &status);
  if(flag){ 
		MPI_Get_count(&status, MPI_BYTE, &size);
		storage = malloc(size);
    MPI_Recv(storage, size, MPI_BYTE, MPI_ANY_SOURCE, actor->id, MPI_COMM_WORLD, &status);
		info = (int *) storage;
		actor->sender = info[0];
		actor->act_number = info[1];
		actor->last_message_size = info[2];
		if(actor->act_number == 0){
			actor->sent_props = NULL;
		}
		else{
			actor->sent_props = info+3;
		}
		if(actor->act_number == -1)
		{
			actor->poison_pill = 1;
		}
	}
	return;
}
