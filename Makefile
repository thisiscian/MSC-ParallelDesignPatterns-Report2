CC=gcc
SOURCE=main.c
TARGET=main
OBJECT=$(SOURCE:.c=.o)
CFLAGS=

all: $(TARGET)

$(TARGET): $(OBJECT)
	$(CC) $(CFLAGS) -o $@ $(OBJECT)

clean: 
	@rm -f $(TARGET) $(OBJECT)

$(OBJECT): Makefile 

