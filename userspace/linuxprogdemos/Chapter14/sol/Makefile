# Makefile for Chapter 1 labs

TARGETS = nswhod nswho webclient webserv

RM = /bin/rm -f
CC = cc -g
LIBS = 

all: $(TARGETS)

clean:
	$(RM) $(TARGETS) *.o

nswho: nswho.o
	$(CC) nswho.o -o nswho $(LIBS)

nswhod: nswhod.o
	$(CC) nswhod.o -o nswhod $(LIBS)

nswhod.o: nswho.h
nswho.o: nswho.h

webclient: webclient.o
	$(CC) webclient.o -o webclient $(LIBS)

webserv: webserv.o
	$(CC) webserv.o -o webserv $(LIBS)

webserv.o: web.h
webclient.o: web.h
