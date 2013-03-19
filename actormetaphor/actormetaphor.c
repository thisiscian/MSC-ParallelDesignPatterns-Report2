//
// Part of the `actormetaphor` library
//
// Functions used to call the Actor Model using MPI
// Each MPI process spawns at least one actor
// However, each MPI process can have any amount of actors, by having the
// initial actor spawn more

#include "actormetaphor.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// global variable currently needed for unique ID of actors
int next_id = 0;
int process_rank;
int number_of_processes;
int buff_size=100*sizeof(double);

// Define the NULL ROLE, an empty role for when you don't immediately want to
// define what an actor will be doing.
Role NULL_ROLE = {NULL,NULL,0};

// Initialise MPI, and generate one actor per process, with a role chosen 
// according to the function `choose_role`
Actor* actor_initialise_metaphor (Role (*choose_role)(int id))
{
 	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);
	MPI_Buffer_attach(malloc(buff_size),buff_size);
	int id = get_next_id();
	Actor *actor;
	
  if(choose_role == NULL)
  {
    actor = _train_actor(NULL, id, NULL_ROLE);
  }
  else
  {
    actor = _train_actor(NULL, id, choose_role(id));
  }
	MPI_Barrier(MPI_COMM_WORLD);
	return actor;
}

// Receive any leftover messages and then finalise MPI
void actor_finalise_metaphor(){
	int flag;
	int count;
	MPI_Status status;
	void *var;
	MPI_Iprobe(process_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
	while(flag)
	{
		MPI_Get_count(&status,MPI_INT, &count);
		var = malloc(count*sizeof(MPI_INT));
		MPI_Recv(var,count,MPI_INT,process_rank,MPI_ANY_TAG,MPI_COMM_WORLD, &status);
		MPI_Iprobe(process_rank, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
	}
  MPI_Finalize();
}

// Run your current task, and then run the jobs of any proteges (child actors) 
// underneath you. If you are not the lead actor, perform only once.
// If you are the lead actor of a given process, then keep looping until there
// are no active jobs left.
int perform(Actor *actor)
{
  int performances = 0;
  do
  {
		// If you are the lead actor, you must keep looping until there are no jobs
		// left to run
    if(actor->mentor == NULL)
    {
      performances = 0;
    }
		// If you are not retired, run the job, look for messages and run the job
		// assigned to you.
    if(!actor->retire)
    {
  		respond_to_dialogue(actor);
      actor->script(actor);
      performances++;
    }
		// If you receive a poison_pill, immediately quit, and if you are not the 
		// lead actor, send your mentor (parent actor) a poison_pill too.
		if(actor->poison_pill)
		{
			if(actor->mentor!= NULL)
			{
				actor->mentor->poison_pill = 1;
			}
			return 0;
		}
		// Tell your proteges (child actor) to run their jobs, and count up the 
		// amount of performances that complete
    performances += _help_proteges(actor);
  }
	// If you are the lead actor and there are still actors running jobs,
	// restart your loop
  while(performances > 0 && actor->mentor == NULL);
  return performances;
}

// Runs the jobs of protege actors
int _help_proteges(Actor *actor)
{
  int protege_count = 0;
  int current_protege = 0;
	// If you get a poison pill or there are no proteges left, stop working
  while(actor->proteges != NULL && !actor->poison_pill)
  {
    if(perform(actor->proteges->actor))
    {
      protege_count++;
      actor->proteges = actor->proteges->next;
    }
    else
    {
			// If the protege has retied, remove him from the list
      _retire_protege(actor);
    }
  }
	// return to the top of the protege list
  actor->proteges = actor->first_protege;
  return protege_count;
}

void _retire_protege(Actor* actor)
{
  Protege* retiree = actor->proteges;
  actor->proteges = actor->proteges->next;
  if(retiree == actor->first_protege)
  {
    actor->first_protege = actor->proteges;
  }
}

int is_null_role(Role role)
{
	int boole = role.script == NULL_ROLE.script;
	boole &= role.rehearse == NULL_ROLE.rehearse;
	boole &= role.memory_required == NULL_ROLE.memory_required;
	return boole;
}

Actor* _train_actor (Actor* new_mentor, int new_id, Role role)
{
  Actor *actor = (Actor*) malloc(sizeof(Actor));
  actor->id = new_id;
  actor->retire = 0;
  actor->mentor = new_mentor;
  actor->first_protege = NULL;
  actor->proteges = NULL;
	actor->sender = -1;
	actor->poison_pill = 0;
	if(!is_null_role(role))
	{
		actor->script = role.script;
  	actor->rehearse = role.rehearse;
  	actor->props = malloc(role.memory_required);
  	actor->rehearse(actor);
	}
	else
	{
		actor->script = no_script;
		actor->rehearse = no_rehearse;
  	actor->props = malloc(0);
	}
  return actor;
}

// creates a new Protege which has no following Protege
Protege* _train_protege(Actor* new_actor)
{
  Protege* actor_list;
  if(!(actor_list = malloc(sizeof(Protege)))) return NULL;
  actor_list->next=NULL;
  actor_list->actor = new_actor;
  return actor_list;
}

// creates a new Protege which links to another list
Protege* _add_protege_to_proteges(Actor* new_actor, Protege* new_list)
{
  Protege* actor_list = _train_protege(new_actor);
  actor_list->next = new_list;
  return actor_list;
}
int peek_next_id()
{
	int rank, size, id;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	id=rank+next_id*size; 
	return id;
}

int get_next_id()
{
	int rank, size, id;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	id=rank+next_id*size; 
  next_id++;
	return id;
}

Actor* actor_train_protege (Actor* mentor, Role role)
{
  Actor *actor;
	actor = _train_actor(mentor, get_next_id(), role);
  if(mentor->proteges != NULL)
  {
    mentor->proteges->next = _add_protege_to_proteges(actor, mentor->proteges->next);
  }
  else 
  {
    mentor->proteges = _add_protege_to_proteges(actor,NULL);
    mentor->first_protege = mentor->proteges;
  }
  return actor;
}

void enter_dialogue(Actor* actor, int your_id, int message)
{
  int destination_rank;
  int message_holder[2] = {message, actor->id}; 

	destination_rank = your_id%number_of_processes;
	MPI_Bsend(message_holder, 2, MPI_INT, destination_rank, your_id+1, MPI_COMM_WORLD);
}

void enter_dialogue_with_all_proteges(Actor* actor, int message)
{
  Protege* protege = actor->first_protege;
  while(protege != NULL)
  {
    enter_dialogue(actor, protege->actor->id, message);
    protege = protege->next;
  }
}

void give_props(Actor* actor, int your_id, int prop_count, MPI_Datatype datatype, void* prop)
{
  int destination_rank;
  destination_rank = your_id%number_of_processes;
	MPI_Bsend(prop, prop_count, datatype, destination_rank, your_id+1, MPI_COMM_WORLD);
}

void give_props_to_all_proteges(Actor* actor, int prop_count, MPI_Datatype datatype, void* prop)
{
  Protege* protege = actor->first_protege;
  while(protege != NULL)
  {
    give_props(actor, protege->actor->id, prop_count, datatype, prop);
    protege = protege->next;
  }
}

void respond_to_dialogue(Actor* actor)
{
  int message[2] = {-1,-1};
	int flag;
  MPI_Status status;
	MPI_Iprobe(MPI_ANY_SOURCE, actor->id+1, MPI_COMM_WORLD, &flag, &status);
  if(flag)
  { 
    MPI_Recv(&message, 2, MPI_INT, MPI_ANY_SOURCE, actor->id+1, MPI_COMM_WORLD, &status);
		actor->act_number = message[0];
		actor->sender = message[1];
	}
}

void get_props(Actor* actor, int prop_count, MPI_Datatype datatype, void* prop)
{
	int flag;
	int count;
	MPI_Status status = {-1,-1,-1};

	MPI_Iprobe(MPI_ANY_SOURCE, actor->id+1, MPI_COMM_WORLD, &flag, &status);
	MPI_Get_count(&status, datatype, &count);
	if(count == prop_count)
	{
		MPI_Recv(prop, prop_count, datatype, MPI_ANY_SOURCE, actor->id+1, MPI_COMM_WORLD, &status);
	}
}

// No initialisation for this actor
void no_rehearse(Actor* actor) {}

// No task for this actor
void no_script(Actor* actor)
{
  actor->retire = 1;
}
