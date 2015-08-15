all:
	gcc imageBlocking.c -Wall -g -O2 -o imageBlocking
	gcc blockAssembler.c -Wall -g -O2 -o assemble
	gcc yuvQuantizer.c -Wall -g -O2 -o quantizer
	gcc psnrCalculation.c bmpReader.c -Wall -g -O2 -lm -o psnr

clean:
	rm imageBlocking
	rm assemble
	rm quantizer
	rm psnr

