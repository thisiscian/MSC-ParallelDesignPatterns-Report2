#include <stdio.h>
#include "actor/actor.h"

void act_init();
void act_start();
void act_stop();

int main()
{
  actor baby;
  baby.init = act_init;
  baby.start = act_start;
  baby.stop = act_stop;

  baby.init();
  baby.start();
  baby.stop();
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
