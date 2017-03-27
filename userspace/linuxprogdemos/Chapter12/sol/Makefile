# Makefile for Chapter 1 labs

TARGETS = filesrv fileclnt

RM = /bin/rm -f
CC = cc -g

all: $(TARGETS)

clean:
	$(RM) $(TARGETS) *.o

filesrv: filesrv.o semops.o
fileclnt: fileclnt.o semops.o

fileclnt.o: sem.h
filesrv.o:  sem.h
