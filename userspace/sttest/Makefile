
CFLAGS=-g
LDFLAGS=-lpthread

.PHONY: clean all

all: test

x.o: x.c 
	$(CC) $(LDFLAGS) -c x.c

test: x.o
	$(CC) $(LDFLAGS) x.o -o test

clean:
	@rm -f *.o *~test