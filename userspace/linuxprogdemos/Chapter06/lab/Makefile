# Makefile for Chapter 1 labs

TARGETS = clock anim

RM = /bin/rm -f
CC = cc -g

all: $(TARGETS)

clock: clock.c
	cc -o clock clock.c -lcurses

anim: anim.c
	cc -o anim anim.c -lcurses

clean:
	$(RM) $(TARGETS)
