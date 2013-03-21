VTCC=vtcc -vt:cc mpicc
MPICC=mpicc
GCC=mpicc -cc=gcc -Wall
SOURCE_FOLDER=src
HEADER_FOLDER=header
SOURCE=$(wildcard $(SOURCE_FOLDER)/*.c)
HEADER=$(wildcard $(HEADER_FOLDER)/*.h) actor_metaphor.h
TARGET=libactor_metaphor.a
OBJECT=$(patsubst $(SOURCE_FOLDER)/%.c,%.o,$(SOURCE))
CFLAGS=-g

mpicc: $(OBJECT)
	@echo "  ##Making actor_metaphor with mpicc##  "
	$(MPICC) -c $(SOURCE) $(CFLAGS)
	@ar -rs $(TARGET) $(OBJECT) $(HEADER)
	@rm -f $(OBJECT)

vampir: $(OBJECT)
	@echo "  ##Making actor_metaphor library for vampirs##  "
	$(VTCC) -c $(SOURCE) $(CFLAGS)
	@ar -rs $(TARGET) $(OBJECT) $(HEADER)
	@rm -f $(OBJECT)

gcc: $(OBJECT)
	@echo "  ##Making actor_metaphor library with gcc flags##  "
	$(GCC) -c $(SOURCE) $(CFLAGS)
	@ar -rs $(TARGET) $(OBJECT) $(HEADER)
	@rm -f $(OBJECT)
	
clean: 
	@echo "Cleaning actormetaphor folder..."
	@rm -f $(TARGET) $(OBJECT)

$(OBJECT): Makefile

