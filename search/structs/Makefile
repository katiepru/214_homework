CC = gcc
CFLAGS = -ggdb -Wall
AR = ar

all: ../libstructs.a

../libstructs.a: sorted-list.o file-node.o trie.o
	$(AR) rv ../libstructs.a sorted-list.o file-node.o trie.o

trie.o: trie.c
	$(CC) $(CFLAGS) -c trie.c

file-node.o: file-node.c
	$(CC) $(CFLAGS) -c file-node.c

sorted-list.o: sorted-list.c
	$(CC) $(CFLAGS) -c sorted-list.c

clean:
	@rm -rf *.o
	@rm -rf ../libstructs.a
