CC=vtcc
ACTOR_DIR=../actormetaphor
ACTOR_LIB=actormetaphor
SOURCE=test.c
TARGET=test_exec
OBJECT=$(SOURCE:.c=.o)
CFLAGS=-lm -l$(ACTOR_LIB) -L$(ACTOR_DIR) -I$(ACTOR_DIR) -I/opt/mpich2-gnu/include -L/opt/mpich2-gnu/lib -lmpich -lopa -lmpl -lrt -lpthread

all: $(TARGET)

$(TARGET): $(OBJECT)
	$(CC) -o $@ $(OBJECT) $(CFLAGS) 

clean: 
	@echo "Cleaning test folder..."
	@rm -f $(TARGET) $(OBJECT)

run: $(TARGET)
	mpiexec -n 2 ./$(TARGET)

$(OBJECT): Makefile 

