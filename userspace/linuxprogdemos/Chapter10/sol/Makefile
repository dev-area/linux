# Makefile for Chapter 1 labs

TARGETS = msgfsrv msgfclnt

RM = /bin/rm -f
CC = cc -g

all: $(TARGETS)

clean:
	$(RM) $(TARGETS) *.o

msgfsrv.o: msg.h
msgfclnt.o: msg.h
