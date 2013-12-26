CC=g++
CFLAGS=-g
CPPFLAGS=-g

all: avltest

avltest: avltest.o avltree.o

clean:
	rm *.o avltest
