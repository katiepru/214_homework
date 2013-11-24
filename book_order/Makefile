CC = gcc
CFLAGS = -ggdb -Wall -pthread

all : book-order

book-order: libstructs.a book-order.o
	$(CC) $(CFLAGS) -o book-order book-order.o libstructs.a

book-order.o: book-order.c book-order.h
	$(CC) $(CFLAGS) -c book-order.c

libstructs.a :
	@cd structs; $(MAKE) $(MFLAGS)

clean :
	@rm -f *.o *.a
	@cd structs && rm -f *.o *.a
	@rm -f book-order libstructs.a
