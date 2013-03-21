/*****************************************************************************/
/* Part of the `actor_metaphor` library - an MPI implementation of the actor */
/* model written in C                                                        */
/*****************************************************************************/

#include "../header/am_interact.h"
#include "../header/am_misc.h"
#include "../header/am_proteges.h"

// Allows actors to send messages and data to each other via MPI
// - storage is a char* type pointer because chars are 1 byte, which makes it easy to send flexible messages
void interact(Actor *actor, int interact_id, int message, int message_size, MPI_Datatype datatype, void *prop){
	int size = 3*sizeof(int)+message_size*sizeof(datatype)+sizeof(MPI_Datatype);
	int destination_rank = interact_id%number_of_processes;

	char* storage = malloc(size);
	char* props = (char*) prop;

	*(storage+0*sizeof(int)) = actor->id;
	*(storage+1*sizeof(int)) = message;
	*(storage+2*sizeof(int)) = message_size;
	if(message_size > 100 || interact_id < 0)
	{
		printf("Actor(%d): message is crazy:\n\tmessage_size=%d\n\tmessage=%d\n\tinteract_id=%d\n", actor->id, message_size, message, interact_id);
		actor->poison_pill=1;
		return;
	}
	if(message_size != 0 && prop != NULL){
		*(storage+3*sizeof(int)+sizeof(MPI_Datatype)) = *props;
	}

	MPI_Bsend(storage, size, MPI_BYTE, destination_rank, interact_id, MPI_COMM_WORLD);
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
	char *storage;
  MPI_Status status;
	MPI_Iprobe(MPI_ANY_SOURCE, actor->id, MPI_COMM_WORLD, &flag, &status);
	MPI_Get_count(&status, MPI_BYTE, &size);
  if(flag){ 
		storage = malloc(size);
    MPI_Recv(storage, size, MPI_BYTE, MPI_ANY_SOURCE, actor->id, MPI_COMM_WORLD, &status);
		actor->sender = *storage;
		actor->act_number = *(storage+1*sizeof(int));
		actor->last_message_size = *(storage+2*sizeof(int));
		if(actor->last_message_size == 0){
			actor->sent_props = NULL;
		}
		else{
			actor->sent_props = (char*) (storage + 3*sizeof(int)+sizeof(MPI_Datatype));
		}
	}
	return;
}
