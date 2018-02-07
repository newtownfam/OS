CC = gcc
CFLAGS = -Wall
OBJS = Bathroom.o brTest.o

all: $(OBJS)
    $(CC) $(CFLAGS) $(OBJS) -o sysstatd

server.o: server.c
    $(CC) $(CFLAGS) -c server.c

rio.o: rio.c rio.h
    $(CC) $(CFLAGS) -c rio.c

clean:
    rm -f *~ *.o sysstatd