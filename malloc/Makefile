CC = gcc
CFLAGS = -ggdb -Wall

all : my_malloc.o tests

my_malloc.o: my_malloc.c my_malloc.h
	$(CC) $(CFLAGS) -c my_malloc.c

tests: tests.c my_malloc.o
	$(CC) $(CFLAGS) -o tests tests.c

clean :
	@rm -f *.o *.a
	@rm -f tests
