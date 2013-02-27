/*
 *    Define a new struct called 'Actor'; contains MPI information and function pointers to tell it what to run
 *
 */

#ifndef __PDP_ACTOR_H__
#define __PDP_ACTOR_H__

typedef struct {
  int unique_id;
  int mpi_rank;
  int mpi_size;
  int kill;
  void (*start)();
  void (*stop)();
} Actor;

Actor* initActor();
void actor_kill(Actor *actor);

void actors_init();
void actors_finalize();

void spawnActor();

#endif
