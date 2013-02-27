CC=mpiCC
SOURCE=test.cpp
TARGET=test
OBJECT=$(SOURCE:.c=.o)
CFLAGS=

all: $(TARGET)

$(TARGET): $(OBJECT)
	$(CC) $(CFLAGS) -o $@ $(OBJECT)

clean: 
	@rm -f $(TARGET) $(OBJECT)

$(OBJECT): Makefile 

