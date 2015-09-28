all:
	gcc imageBlocking.c -Wall -g -O2 -o imageBlocking
	gcc blockAssembler.c -Wall -g -O2 -o assemble
	gcc yuvQuantizer.c -Wall -g -O2 -o quantizer
	gcc psnrCalculation.c bmpReader.c -Wall -g -O2 -lm -o psnr
	gcc entropyFinder.c bmpReader.c -Wall -g -O2 -lm -o entropy
	gcc downSampler.c bmpReader.c -Wall -g -O2 -lm -o downSampler
	gcc dct.c bmpReader.c -Wall -g -O2 -lm -o dct
	gcc huffmanImageCoding.c -Wall -g -O2 -lm -o huff

clean:
	rm imageBlocking
	rm assemble
	rm quantizer
	rm psnr
	rm entropy
	rm downSampler
	rm dct
	rm huff
