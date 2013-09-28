CC = gcc
CFLAGS = -ggdb -Wall
AR = ar

all : liblist.a

sorted-list.o : sorted-list.c
	$(CC) $(CFLAGS) -c sorted-list.c

liblist.a : sorted-list.o
	$(AR) rv liblist.a sorted-list.o

clean :
	@rm -f *.o *.a
