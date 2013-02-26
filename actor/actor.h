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
} actor;

#endif
