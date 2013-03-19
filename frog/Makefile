CC=cc
ACTOR_DIR=../actormetaphor
ACTOR_LIB=actormetaphor
PROVIDED_DIR=provided-functions
SRC_DIR=src

SOURCE=main.c
EXTRA=$(PROVIDED_DIR)/frog-functions.c $(PROVIDED_DIR)/ran2.c $(SRC_DIR)/frogs.c $(SRC_DIR)/timers.c $(SRC_DIR)/land_cells.c $(SRC_DIR)/extra_functions.c
TARGET=frog_exec
OBJECT=$(SOURCE:.c=.o)
EXTRA_OBJECT=$(EXTRA:.c=.o)
CFLAGS= -g -lm -l$(ACTOR_LIB) -L$(ACTOR_DIR) -I$(ACTOR_DIR)

all: $(TARGET)

$(TARGET): $(OBJECT) $(EXTRA_OBJECT)
	$(CC) -o $@ $(OBJECT) $(CFLAGS) $(EXTRA_OBJECT)

clean: 
	@echo "Cleaning frog folder..."
	@rm -f $(TARGET) $(OBJECT) $(EXTRA_OBJECT)

run: $(TARGET)
	mpiexec -n 2 ./$(TARGET)

$(OBJECT): Makefile 

$(EXTRA_OBJECT): Makefile 

