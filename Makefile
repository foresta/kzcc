kzcc: kzcc.c

test: kzcc
	./kzcc -test
	./test.sh

clean:
	rm -rf kzcc *.o *~ tmp*
