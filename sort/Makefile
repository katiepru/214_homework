CC = gcc
CFLAGS = -ggdb -Wall
AR = ar

all : main

main: liblist.a main.o
	$(CC) $(CFLAGS) -o sl main.o liblist.a

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

sorted-list.o : sorted-list.c
	$(CC) $(CFLAGS) -c sorted-list.c

liblist.a : sorted-list.o
	$(AR) rv liblist.a sorted-list.o

clean :
	@rm -f *.o *.a
	@rm -f main
