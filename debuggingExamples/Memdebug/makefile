FLAGS=-g
CC=gcc
SRC=.
OBJ=.

all: Bad1 Bad2 Bad3

Bad1:  $(SRC)/Bad1.c ${SRC}/mcb.c ${SRC}/dbm.h
	$(CC) $(FLAGS) $(SRC)/Bad1.c ${SRC}/mcb.c -o $(OBJ)/Bad1

Bad2:  $(SRC)/Bad2.c ${SRC}/mcb.c ${SRC}/dbm.h
	$(CC) $(FLAGS) $(SRC)/Bad2.c ${SRC}/mcb.c -o $(OBJ)/Bad2

Bad3:  $(SRC)/Bad3.c ${SRC}/mcb.c ${SRC}/dbm.h
	$(CC) $(FLAGS) $(SRC)/Bad3.c ${SRC}/mcb.c -o $(OBJ)/Bad3

tidy:
	-rm Bad1 Bad2 Bad3

