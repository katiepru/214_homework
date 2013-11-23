CC = gcc
CFLAGS = -ggdb -Wall
AR = ar

all: ../libstructs.a

../libstructs.a: synch-queue.o trie.o
	$(AR) rv ../libstructs.a synch-queue.o trie.o

trie.o: trie.c
	$(CC) $(CFLAGS) -c trie.c

synch-queue.o: synch-queue.c synch-queue.h
	$(CC) $(CFLAGS) -c synch-queue.c

clean:
	@rm -rf *.o
	@rm -rf ../libstructs.a
