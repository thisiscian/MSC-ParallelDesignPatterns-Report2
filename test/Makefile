CC=mpicc
ACTOR_DIR=../actormetaphor
ACTOR_LIB=actormetaphor
SOURCE=test.c
TARGET=test_exec
OBJECT=$(SOURCE:.c=.o)
CFLAGS=-Wall -lm -l$(ACTOR_LIB) -L$(ACTOR_DIR) -I$(ACTOR_DIR)

all: $(TARGET)

$(TARGET): $(OBJECT)
	$(CC) -o $@ $(OBJECT) $(CFLAGS) 

clean: 
	@echo "Cleaning test folder..."
	@rm -f $(TARGET) $(OBJECT)

run: $(TARGET)
	mpiexec -n 2 ./$(TARGET)

$(OBJECT): Makefile 

