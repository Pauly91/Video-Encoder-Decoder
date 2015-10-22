#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmpReader.h"

#define MAX 255

int main(int argc, char const *argv[])
{
	if (argc != 3)
	{
		printf(" ./downSampler [image1] [type of downSampling - 444 422 411 421] \n");
		return -1;
	}
	BMPData *image1 = NULL;
	int i = 0,j =0, k =0;
	int dSampleU = 0;
	int dSampleV = 0;
	int DsampleType = 0	;
	unsigned char **red = NULL,**green = NULL,**blue = NULL;
	unsigned char **Y = NULL,**U = NULL,**V = NULL;

	
	if((image1 = readBMPfile(argv[1])) == NULL)
	{
		printf("Error in File 1\n");
		return -1;
	}
	DsampleType = atoi(argv[2]);
	dSampleV = DsampleType%10;
	DsampleType /= 10;
	dSampleU = DsampleType%10;
	printf("dSampleU:%d dSampleV:%d\n",dSampleU,dSampleV);

	red = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );
	blue = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );
	green = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );
	Y = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );
	U = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );
	V = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );

	for (i = 0; i < image1->infoHeader.height; ++i)
	{
		red[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );
		blue[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );
		green[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );
		Y[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );
		U[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );
		V[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );		
	}

	colourVector2MatrixConverter(red,green,blue,image1->bitMapImage,image1->infoHeader.height,image1->infoHeader.width);
	rgb2yuv(red,green,blue,Y,U,V,image1->infoHeader.height,image1->infoHeader.width);

	if(dSampleU != 4 && dSampleV  != 4)
	{
		if(dSampleU == 1)
			dSampleU = 4;
		if(dSampleV == 1)
			dSampleV = 4;	
		if(dSampleU == dSampleV)
		{
			for (i = 0; i < image->infoHeader.height; ++i) // check loop
			{
				for (j = 0; j < image->infoHeader.width; j += dSampleU) // check loop
				{
					for (k = 1; k < dSampleU; ++k)
					{
					 	U[i][j+k] = U[i][j];
					 	V[i][j+k] = V[i][j];

					}	 
				}
			}
		}
		else
		{
			if(dSampleU == 4)
			{
				dSampleU = 2;
				dSampleV = 0;
			}
			else if(dSampleU == 2)
			{
				dSampleV = 2;
			}
			for (i = 0; i < image->infoHeader.height; i += dSampleU) // check loop
			{
				for (j = 0; j < image->infoHeader.width; j += dSampleU) // check loop
				{
					for (k = 0; k < dSampleU; ++k)
					{
					 	for (l = 0; l < dSampleV; ++l)
					 	{
						 	U[i + k][j + l] = U[i][j];
						 	V[i + k][j + l] = V[i][j];
					 	}
					}

				}
			}		
		}

	}

	yuv2rgb(red,green,blue,Y,U,V,image1->infoHeader.height,image1->infoHeader.width);
	colourMatrix2VectorConverter(red,green,blue,image1->bitMapImage,image1->infoHeader.height,image1->infoHeader.width);

	if((writeBMPfile(image1,"downSampled.bmp")) != 1)
	{
		printf(" Error Writing BMP file\n");

		free(red);
		free(green);
		free(blue);
		free(Y);
		free(U);
		free(V);
	}

	free(red);
	free(green);
	free(blue);
	free(Y);
	free(U);
	free(V);
	return 0;
}