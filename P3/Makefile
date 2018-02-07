# Project 3 Makefile
# Peter Christakos
# Andrew Morrison
CC = gcc
CFLAGS = -Wall
OBJS = Bathroom.o brTest.o
OBJS2 = Bathroom.o bathSim.o

all: brTest bathSim  # makes two exe, tester and sim

brTest: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o brTest

brTest.o: brTest.c
	$(CC) $(CFLAGS) -c brTest.c

bathSim: $(OBJS2)
	$(CC) $(CFAGS) $(OBJS2) -o bathSim

bathSim.o: bathSim.c
	$(CC) $(CFLAGS) -c bathSim.c

Bathroom.o: Bathroom.c Bathroom.h  #used for both exe
	$(CC) $(CFLAGS) -c Bathroom.c

clean:
	rm -f *~ *.o brTest bathSim