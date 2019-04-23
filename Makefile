kzcc: kzcc.c

test: kzcc
		./test.sh

clean:
		rm -rf kzcc *.o *~ tmp*
