CC=gcc
SOURCE=main.c
TARGET=main
OBJECT=$(SOURCE:.c=.o)
LIBRARY=actor/actor.c
CFLAGS=

all: $(TARGET)

$(TARGET): $(OBJECT) $(LIBRARY)
	$(CC) $(CFLAGS) -o $@ $(SOURCE) $(LIBRARY) -lm

clean: 
	@rm -f $(TARGET) $(OBJECT)

$(OBJECT): Makefile 
