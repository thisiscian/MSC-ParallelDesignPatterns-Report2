/*
 *    Define a new struct
 *    called Actor
 *
 */

#ifndef __PDP_ACTOR_H__
#define __PDP_ACTOR_H__

struct Actor_s{
  int unique_id;
  int mpi_rank;
  int mpi_size;
  int kill_now;
  void (*init)();
  void (*start)();
  void (*stop)();
  void (*kill)(struct Actor_s this);
};
typedef struct Actor_s Actor;

void actor_kill(struct Actor_s this);

void actors_init();
void actors_finalize();

void message_actor(Actor them, int message);
void spawn_child_actor(Actor);
#endif
