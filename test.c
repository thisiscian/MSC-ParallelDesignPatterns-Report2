#include <mpi.h>
#include <iostream>
#include "actor/actor.h"

void act_init();
void act_start();
void act_stop();

typedef struct {
  int infectionValue;
  int currentHop;
} Frog;

using namespace std;

int main()
{
  Actor actor;
  Frog frog = {0,0};

  actor.my_var = &frog;

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
  cout << "init" << endl;
}
void act_start()
{
  cout << "start" << endl;
}
void act_stop()
{
  cout << "stop" << endl;
}
