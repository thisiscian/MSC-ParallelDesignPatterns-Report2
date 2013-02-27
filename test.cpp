#include <stdio.h>
#include "actor/actor.h"

void act_init();
void act_start();
void act_stop();

typedef struct {
  int infectionValue;
  int currentHop;
} Frog;

int main()
{
  Actor actor;
  Frog frog = {0,0};

  actor.my_var = &frog;
  actor.my_struct = Frog*;

  printf("%d\n", ((Frog*) actor.my_var)->currentHop);

  actor.init = act_init;
  actor.start = act_start;
  actor.stop = act_stop;

  actor.init();
  actor.start();
  actor.stop();
  
  return 0;
}

void act_init()
{
  printf("Init\n");
}
void act_start()
{
  printf("Start\n");
}
void act_stop()
{
  printf("Stop\n");
}
