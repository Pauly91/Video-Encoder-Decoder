#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmpReader.h"

#define MAX 255

int main(int argc, char const *argv[])
{
	if (argc != 3)
	{
		printf(" ./psnr [image1] [image2]\n");
		return -1;
	}
	BMPData *image1 = NULL,*image2 = NULL;
	int i = 0;
	int meanSquareError = 0;
	float PSNR = 0;
	if((image1 = readBMPfile(argv[1])) == NULL)
		printf("Error in File 1\n");
	if((image2 = readBMPfile(argv[2])) == NULL)
		printf("Error in File 2\n");

	for (i = 0; i < image1->infoHeader.height * image1->infoHeader.width; ++i)
	{
			meanSquareError += (image1->bitMapImage[i] - image2->bitMapImage[i])*(image1->bitMapImage[i] - image2->bitMapImage[i]); 
			printf("MSE:%d Pixel1: %d Pixel2: %d\n",meanSquareError,image1->bitMapImage[i],image2->bitMapImage[i]);
	}
	printf("MSE: %d h: %d w: %d\n",meanSquareError,image1->infoHeader.height,image1->infoHeader.width);
	if(meanSquareError == 0)
	{
		printf("The Images are the same\n");
		return 0;
	}
	meanSquareError = meanSquareError/(image1->infoHeader.height * image1->infoHeader.width);



	PSNR = 10 * log10((MAX * MAX)/meanSquareError);
	printf("PSNR : %f\n",PSNR);
	return 0;
}