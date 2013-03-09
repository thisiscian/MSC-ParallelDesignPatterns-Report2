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
int next_id = 1;

// Define the NULL ROLE, an empty role

// Initialise MPI and create one actor per process, who's id is the rank of the
// host process. The input variables are functions which returns function
// pointers; this means that you can functionally choose which script each 
// actor reads from, when you start the model.
Actor* actor_initialise_metaphor (Role (*choose_role)(int id))
{
  int mpi_rank;
  MPI_Init(NULL,NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  Role NULL_ROLE = {NULL,NULL,0};
  if(choose_role == NULL)
  {
    return _train_actor(NULL, mpi_rank, NULL_ROLE);
  }
  else
  {
    return _train_actor(NULL, mpi_rank, choose_role(mpi_rank));
  }
}

// void FINALISE_MODEL
// finalise MPI and the actor model
void actor_finalise_metaphor(){
  MPI_Finalize();
}

// void START
// Starts a particular actor doing their normal work. This consists of reading
// their 'script' and then starting any children it may have. Special case for
// the first actors on an mpi process; they keep running until they are told
// to stop. Other actors only run their start process once .
int perform(Actor *actor)
{
  int performances = 0;
  do
  {
    if(actor->mentor == NULL)
    {
      performances = 0;
    }
    if(!actor->retire)
    {
      actor->script(actor);
      performances++;
    }
    performances += _help_proteges(actor);
  }
  while(performances > 0 && actor->mentor == NULL);
  if(actor->mentor == NULL || performances == 0)
  {
     _retire_actor(actor);
  }
  return performances;
}

int _help_proteges(Actor *actor)
{
  int protege_count = 0;
  int current_protege = 0;
  while(actor->proteges != NULL)
  {
    if(perform(actor->proteges->actor))
    {
      protege_count++;
      actor->proteges = actor->proteges->next;
    }
    else
    {
      _retire_protege(actor);
    }
  }
  actor->proteges = actor->first_protege;
  return protege_count;
}

void _retire_actor(Actor* actor)
{
  free(actor->props);
  free(actor);
}

void _retire_protege(Actor* actor)
{
  Protege* retiree = actor->proteges;
  actor->proteges = actor->proteges->next;
  if(retiree == actor->first_protege)
  {
    actor->first_protege = actor->proteges;
  }
  _retire_actor(retiree->actor);
  free(retiree);
}

Actor* _train_actor (Actor* new_mentor, int new_id, Role role)
{
  Actor *actor = (Actor*) malloc(sizeof(Actor));
  actor->id = new_id;
  MPI_Comm_rank(MPI_COMM_WORLD, &(actor->mpi_rank));
  MPI_Comm_size(MPI_COMM_WORLD, &(actor->mpi_size));
  actor->retire = 0;
  actor->script = role.script;
  actor->rehearse = role.rehearse;
  actor->props = malloc(role.memory_required);
  actor->proteges = NULL;
  actor->mentor = new_mentor;
  actor->first_protege = NULL;
  actor->rehearse(actor);
  return actor;
}

// Protege* NEW_ACTOR_LIST
// creates a new Protege which points to nothing, but has an actor
Protege* _train_protege(Actor* new_actor)
{
  Protege* actor_list;
  if(!(actor_list = malloc(sizeof(Protege)))) return NULL;
  actor_list->next=NULL;
  actor_list->actor = new_actor;
  return actor_list;
}

// Protege* ACTOR_LIST_NEW_LINK
// creates a new Protege which links to another list
Protege* _add_protege_to_proteges(Actor* new_actor, Protege* new_list)
{
  Protege* actor_list = _train_protege(new_actor);
  actor_list->next = new_list;
  return actor_list;
}

int get_next_id(Actor* actor)
{
   return actor->mpi_rank+next_id*actor->mpi_size; 
}

Actor* actor_train_protege (Actor* mentor, Role role)
{
  Actor *actor = _train_actor(mentor, get_next_id(mentor), role);
  next_id++;
  printf("Actor(%d) spawning actor(%d)\n", mentor->id, actor->id);
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

void enter_dialogue(Actor* me, int you, int message_type, int next_message_size)
{
  int destination_rank = you%me->mpi_size;
  int message[2] = {message_type, next_message_size}; 
  MPI_Request request;
  MPI_Isend(message, 2, MPI_INT, destination_rank, you, MPI_COMM_WORLD, &request);
}

void enter_dialogue_with_all_proteges(Actor* actor, int message_type, int next_message_size)
{
  Protege* protege = actor->first_protege;
  while(protege != NULL)
  {
    enter_dialogue(actor, protege->actor->id, message_type, next_message_size);
    protege = protege->next;
  }
}

void respond_to_dialogue(Actor* actor, void (*reaction)(Actor* actor, int message_type, int next_message_size))
{
  int message[2];
  int received;
  MPI_Request request;
  MPI_Status status;
  MPI_Irecv(&message, 2, MPI_INT, MPI_ANY_SOURCE, -1, MPI_COMM_WORLD, &request);
  MPI_Request_get_status(request, &received, &status);
  if(!received)
  { 
    MPI_Irecv(&message, 2, MPI_INT, MPI_ANY_SOURCE, actor->id, MPI_COMM_WORLD, &request);
  }
  MPI_Request_get_status(request, &received, &status);
  if(received)
  {
    reaction(actor, message[0], message[1]);
  }
}

void give_props(Actor* me, int you, int prop_count, MPI_Datatype datatype, void* prop)
{
  int destination_rank = you%me->mpi_size;
  MPI_Request request;
  MPI_Isend(prop, prop_count, MPI_INT, destination_rank, you, MPI_COMM_WORLD, &request);
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

void get_props(Actor* actor, int prop_count, MPI_Datatype datatype, void* prop)
{
  int received;
  MPI_Request request;
  MPI_Status status;
  MPI_Irecv(prop, prop_count, MPI_INT, MPI_ANY_SOURCE, actor->id, MPI_COMM_WORLD, &request);
  MPI_Request_get_status(request, &received, &status);
  if(received)
  {
    printf("I have received\n");
  }
}

// No initialisation for this actor
void no_rehearse(Actor* actor) {}

// No task for this actor
void no_script(Actor* actor)
{
  actor->retire = 1;
}

