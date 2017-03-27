# Makefile for Chapter 1 labs

TARGETS = filesrv fileclnt filesrv2

RM = /bin/rm -f
CC = cc -g
LIBS= 

all: $(TARGETS) 

clean:
	$(RM) $(TARGETS) *.o

filesrv: filesrv.o
	$(CC) -o filesrv filesrv.o $(LIBS)

filesrv2: filesrv2.o
	$(CC) -o filesrv2 filesrv2.o $(LIBS)

fileclnt: fileclnt.o
	$(CC) -o fileclnt fileclnt.o $(LIBS)
