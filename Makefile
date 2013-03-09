CC=mpicc
SOURCE=frog.c actor/actor.c provided-functions/frog-functions.c provided-functions/ran2.c
TARGET=frog
OBJECT=$(SOURCE:.c=.o)
CFLAGS=-Wall

all: $(TARGET)

$(TARGET): $(OBJECT)
	$(CC) $(CFLAGS) -o $@ $(OBJECT)

clean: 
	@rm -f $(TARGET) $(OBJECT)

run: $(TARGET)
	mpiexec -n 2 ./$(TARGET)

$(OBJECT): Makefile 

