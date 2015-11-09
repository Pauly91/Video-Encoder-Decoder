#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmpReader.h"

#define level 256
#define numColor 3


int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		printf(" ./entropy [image1] \n");
		return -1;
	}
	BMPData *image1 = NULL;
	int i;
	float *histogram,entropy = 0.0,check = 0;
	if((image1 = readBMPfile(argv[1])) == NULL)
		printf("Error in File 1\n");

	histogram = (float*) calloc(level,sizeof(float));
	for (i = 0; i < image1->infoHeader.height * image1->infoHeader.width * numColor; ++i)
			histogram[image1->bitMapImage[i]]++;

	for (i = 0; i < level; ++i)
	{
		printf("%d:%f\n",i,histogram[i]);
		histogram[i] = histogram[i] / (image1->infoHeader.height * image1->infoHeader.width * numColor);
		//printf("%d:%f\n",i,histogram[i]);
		if(histogram[i] != 0)
			entropy += histogram[i] * log2(1/histogram[i]); 
	}
	printf("%f\n",entropy);
	return 0;
}