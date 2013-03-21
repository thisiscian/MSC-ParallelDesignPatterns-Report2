CC=mpicc
SOURCE_FOLDER=src
HEADER_FOLDER=header
SOURCE=$(wildcard $(SOURCE_FOLDER)/*.c)
HEADER=$(wildcard $(HEADER_FOLDER)/*.h) actor_metaphor.h
TARGET=libactor_metaphor.a
OBJECT=$(SOURCE:.c=.o)
CFLAGS=-g

mpicc: $(OBJECT)
	@echo -e '\e[1mMaking actor_metaphor library\e[0m'
	@ar -rs $(TARGET) $(OBJECT) $(HEADER)

clean: 
	@echo -e '\e[1mCleaning actormetaphor folder...\e[0m'
	@rm -f $(TARGET) $(OBJECT)

$(OBJECT): Makefile

