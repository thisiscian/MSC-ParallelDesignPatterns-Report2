CC=mpicc
SOURCE_FOLDER=src
HEADER_FOLDER=header
SOURCE=$(wildcard $(SOURCE_FOLDER)/*.c)
HEADER=$(wildcard $(HEADER_FOLDER)/*.h) actor_metaphor.h
TARGET=libactor_metaphor.a
OBJECT=$(SOURCE:.c=.o)
CFLAGS=

mpicc: $(OBJECT)
	@ar -rs $(TARGET) $(OBJECT) $(HEADER)

clean: 
	@rm -f $(TARGET) $(OBJECT)

$(OBJECT): Makefile

