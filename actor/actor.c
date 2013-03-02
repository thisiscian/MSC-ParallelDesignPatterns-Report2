// Functions used to call the Actor Model using MPI
// Each MPI process spawns at least one actor
// However, each MPI process can have any amount of actors, by having the
// initial actor spawn more

#include "actor.h"
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

// global variable currently needed for unique ID of actors
int next_id = 1;

// Actor* INITIALISE_MODEL
// Initialise MPI and create one actor per process, who's id is the rank of the
// host process. The input variables are functions which returns function
// pointers; this means that you can functionally choose which script each 
// actor reads from, when you start the model.
Actor* initialise_actor_model
(
  void (*choose_rehearse_function(int id))(Actor *actor),
  void (*choose_script_function(int id))(Actor* actor),
  int (*choose_amount_of_memory_to_allocate)(int id)
)
{
  int mpi_rank;
  void (*rehearse_function)(Actor *actor);
  void (*script_function)(Actor *actor);
  int amount_of_memory_to_allocate = 0;
  MPI_Init(NULL,NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  if(choose_rehearse_function == NULL)
  {
    rehearse_function = no_rehearse;
  }
  else
  {
    rehearse_function = choose_rehearse_function(mpi_rank);
  }
  if(choose_script_function == NULL)
  {
    script_function = no_script;
  }
  else
  {
    script_function = choose_script_function(mpi_rank);
  }
  if(choose_amount_of_memory_to_allocate == NULL)
  {
    amount_of_memory_to_allocate = 0;
  }
  else
  {
    amount_of_memory_to_allocate = choose_amount_of_memory_to_allocate(mpi_rank);
  }
  return _train_actor(NULL, mpi_rank, rehearse_function, script_function, amount_of_memory_to_allocate);
}

// void FINALISE_MODEL
// finalise MPI and the actor model
void finalise_actor_model(){
  MPI_Finalize();
}

// void START
// Starts a particular actor doing their normal work. This consists of reading
// their 'script' and then starting any children it may have. Special case for
// the first actors on an mpi process; they keep running until they are told
// to stop. Other actors only run their start process once .
int read_script(Actor *actor)
{
  int lines_read = 0;
  do
  {
    if(actor->mentor == NULL)
    {
      lines_read = 0;
    }
    if(!actor->retire)
    {
      actor->script(actor);
      lines_read++;
    }
    lines_read += _help_understudies(actor);
  }
  while(lines_read > 0 && actor->mentor == NULL);
  return lines_read;
}

// void ACTOR_RUN_CHILD_ITERATION
// runs through each child of an actor and tells it to start
int _help_understudies(Actor *actor)
{
  int lines_read = 0;
  while(actor->proteges != NULL)
  {
    lines_read += read_script(actor->proteges->actor);
    actor->proteges = actor->proteges->next;
  }
  actor->proteges = actor->first_protege;
  return lines_read;
}

// void INITIALISE_ACTOR
// initialise an actor with no children, and a given id, script and prop set
Actor* _train_actor
(
  Actor* new_mentor,
  int new_id,
  void (*new_rehearse)(Actor* actor),
  void (*new_script)(Actor* actor),
  int mem_size
)
{
  Actor *actor = (Actor*) malloc(sizeof(Actor));
  actor->id = new_id;
  MPI_Comm_rank(MPI_COMM_WORLD, &(actor->mpi_rank));
  MPI_Comm_size(MPI_COMM_WORLD, &(actor->mpi_size));
  actor->retire = 0;
  actor->script = new_script;
  actor->rehearse = new_rehearse;
  actor->props = malloc(mem_size);
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

// void ACTOR_SPAWN_WITH_ENCORE
// spawns a new actor, with a unique id
Actor* actor_spawn
(
  Actor* mentor,
  void(*rehearse)(Actor* actor),
  void(*script)(Actor* actor),
  int mem_size
)
{
  Actor *actor = _train_actor(mentor, mentor->mpi_rank+next_id*mentor->mpi_size, rehearse, script, mem_size);
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

// void NO_REHEARSAL
// No initialisation for this actor
void no_rehearse(Actor* actor) {}

// void NO_SCRIPT
// No task for this actor
void no_script(Actor* actor)
{
  actor->retire = 1;
}

void read_line(Actor* me, int you, int message_type, int next_message_size)
{
  int destination_rank = you%me->mpi_size;
  int message[2] = {message_type, next_message_size}; 
  MPI_Request request;
  MPI_Isend(message, 2, MPI_INT, destination_rank, you, MPI_COMM_WORLD, &request);
}

void react_to_line(Actor* actor, void (*reaction)(Actor* actor, int message_type, int next_message_size))
{
  int message[2];
  int received;
  MPI_Request request;
  MPI_Status status;
  MPI_Irecv(&message, 2, MPI_INT, MPI_ANY_SOURCE, actor->id, MPI_COMM_WORLD, &request);
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
