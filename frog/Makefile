MPICC=mpicc
VTCC=vtcc -vt:cc mpicc
GCC=mpicc -cc=gcc -Wall
ACTOR_DIR=../actormetaphor
ACTOR_LIB=actor_metaphor
PROVIDED_DIR=provided-functions
SRC_DIR=src

SOURCE=$(wildcard $(PROVIDED_DIR)/*.c) $(wildcard $(SRC_DIR)/*.c)
TARGET=frog_exec
CFLAGS= -g -lm -l$(ACTOR_LIB) -L$(ACTOR_DIR) -I$(ACTOR_DIR)

mpicc:
	@echo "Building $(TARGET) with mpicc"
	$(MPICC) -o $(TARGET) $(SOURCE) $(CFLAGS)

vampir:
	@echo "Building $(TARGET) with vtcc"
	$(VTCC) -o $(TARGET) $(SOURCE) $(CFLAGS)

gcc:
	@echo "Building $(TARGET) with gcc"
	$(GCC) -o $(TARGET) $(SOURCE) $(CFLAGS)

clean: 
	@echo "Cleaning frog folder..."
	@rm -f $(TARGET) $(OBJECT)

run: $(TARGET)
	mpiexec -n 2 ./$(TARGET)
