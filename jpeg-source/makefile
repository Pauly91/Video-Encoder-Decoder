all:
	gcc assessment.c bmpReader.c -Wall -g -O2 -lm -o run
	gcc psnrCalculation.c bmpReader.c -Wall -g -O2 -lm -o psnr
	gcc entropyFinder.c bmpReader.c -Wall -g -O2 -lm -o entropy
clean:
	rm run