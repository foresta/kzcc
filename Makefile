CFLAGS=-Wall -std=c11
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

kzcc: $(OBJS)
		$(CC) -o kzcc $(OBJS) $(LDFLAGS)

$(OBJS): kzcc.h

test: kzcc
	./kzcc -test
	./test.sh

clean:
	rm -rf kzcc *.o *~ tmp*
