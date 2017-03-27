CC=arm-linux-gcc
CFLAGS=-g
LDFLAGS=-lpthread

.PHONY: clean all

all: test

t1.o: t1.c 
	$(CC) $(LDFLAGS) -c t1.c

test: t1.o
	$(CC) $(LDFLAGS) t1.o -o test

clean:
	@rm -f *.o *~test