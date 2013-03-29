CC=mpicc
ACTOR_DIR=../actor_metaphor
ACTOR_LIB=actor_metaphor
PROVIDED_DIR=provided-functions
SRC_DIR=src
SOURCE=$(wildcard $(PROVIDED_DIR)/*.c) $(wildcard $(SRC_DIR)/*.c)
OBJECT=$(SOURCE:.c=.o)
TARGET=frog_exec
CFLAGS=-g -lm -l$(ACTOR_LIB) -L$(ACTOR_DIR) -I$(ACTOR_DIR)

all: $(OBJECT)
	$(CC) -o $(TARGET) $(OBJECT) $(CFLAGS)

clean: 
	@rm -f $(TARGET) $(OBJECT)

run: $(TARGET)
	mpiexec -n 2 ./$(TARGET)

run-%: $(TARGET)
	mpiexec -n $(patsubst run-%,%,$@) ./$(TARGET)

$(OBJECT): Makefile
