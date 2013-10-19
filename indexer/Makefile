CC = gcc
CFLAGS = -ggdb -Wall

all : indexer

indexer: libstructs.a indexer.o
	$(CC) $(CFLAGS) -o index indexer.o fileio.o tokenizer.o libstructs.a

indexer.o: indexer.c indexer.h fileio.o tokenizer.o
	$(CC) $(CFLAGS) -c indexer.c

fileio.o: libstructs.a tokenizer.o
	$(CC) $(CFLAGS) -c fileio.c

tokenizer.o: tokenizer.c tokenizer.h
	$(CC) $(CFLAGS) -c tokenizer.c

libstructs.a :
	@cd structs; $(MAKE) $(MFLAGS)

clean :
	@rm -f *.o *.a
	@rm -f index
