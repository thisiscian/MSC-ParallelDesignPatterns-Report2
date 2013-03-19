CC=mpicc
SOURCE=actormetaphor.c
HEADER=actormetaphor.h
TARGET=libactormetaphor.a
OBJECT=$(SOURCE:.c=.o)
CFLAGS=-g

all:
	@echo "Making actormetaphor library..."
	@mpicc -c $(SOURCE) $(CFLAGS)
	@ar -rs $(TARGET) $(OBJECT) $(HEADER)

clean: 
	@echo "Cleaning actormetaphor folder..."
	@rm -f $(TARGET) $(OBJECT)

$(OBJECT): Makefile $(TARGET)

