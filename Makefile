all:
	gcc imageBlocking.c -Wall -g -O2 -o imageBlocking
	gcc blockAssembler.c -Wall -g -O2 -o assemble
	gcc yuvQuantizer.c -Wall -g -O2 -o quantizer

clean:
	rm imageBlocking
	rm assemble
	rm quantizer

