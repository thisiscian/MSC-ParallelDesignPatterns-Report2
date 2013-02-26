/*
 *    Define a new struct
 *    called Actor
 *
 */

typedef struct actor {
  int unique_id;
  void (*init)();
  void (*start)();
  void (*stop)();
};
