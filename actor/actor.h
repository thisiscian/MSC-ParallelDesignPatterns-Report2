/*
 *    Define a new struct
 *    called Actor
 *
 */

#ifndef __PDP_ACTOR_H__
#define __PDP_ACTOR_H__

typedef struct {
  int unique_id;
  void (*init)();
  void (*start)();
  void (*stop)();
  void (*kill)();

  void *my_var;
  void *my_struct;
} Actor;

void message_actor(Actor them, int message);
void spawn_child_actor(Actor);
#endif
