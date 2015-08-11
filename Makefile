all:
	gcc imageBlocking.c -Wall -g -O2 -o imageBlocking
	gcc blockAssembler.c -Wall -g -O2 -o assemble
	gcc test_imageBlocking.c -Wall -g -O2 -o imageBlocking

clean:
	rm imageBlocking
	rm assemble

