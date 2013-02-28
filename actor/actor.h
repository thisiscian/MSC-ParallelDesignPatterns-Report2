/*
 *    Define a new struct called 'Actor'; contains MPI information and function pointers to tell it what to run
 *
 */

#ifndef __PDP_ACTOR_H__
#define __PDP_ACTOR_H__

typedef struct {
  int unique_id;
  int kill;
  void (*start)();
  void (*stop)();
} Actor;

typedef struct {
  Actor *actor;
  int mpi_rank;
  int mpi_size;
} Director;

void actors_init();
Actor* initActor();
Director* initDirector();

void actor_kill(Actor *actor);
void actors_finalize();
void spawnActor();

#endif
