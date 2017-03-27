# Makefile for Chapter 1 labs

TARGETS = produce consume

RM = /bin/rm -f
CC = cc -g

all: $(TARGETS)

clean:
	$(RM) $(TARGETS) *.o

produce: produce.o semops.o
consume: consume.o semops.o

consume.o: sem.h
produce.o: sem.h
