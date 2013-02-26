CC=mpicc
SOURCE=test.c actor/actor.c
TARGET=test
OBJECT=$(SOURCE:.c=.o)
CFLAGS=-Wall

all: $(TARGET)

$(TARGET): $(OBJECT)
	$(CC) $(CFLAGS) -o $@ $(OBJECT)

clean: 
	@rm -f $(TARGET) $(OBJECT)

$(OBJECT): Makefile 

