#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bmpReader.h"

#define MAX 255
#define pi 3.145


void zigzager(unsigned char ** DCTBlock, int x, int y,int blockSize, FILE *fp)
{
	int i = 0, j = 0;
	for (i = 0; i < blockSize; ++i)
	{
		for (j = 0; j < blockSize; ++j)
		{
			fprintf(fp, "%u",DCTBlock[x+i][y+j]);
		}
	}
}


int main(int argc, char const *argv[])
{
		if (argc != 3)
	{
		printf(" ./dct [image1] [N]  \n");
		return -1;
	}
	BMPData *image1 = NULL;
	FILE * image;
	int i = 0,j = 0, x = 0, y= 0,u = 0,v = 0;
	int blockSize = atoi(argv[2]);
	unsigned char **red = NULL,**green = NULL,**blue = NULL;
	unsigned char **Y = NULL,**U = NULL,**V = NULL;

	float **dct_red = NULL,**dct_green = NULL,**dct_blue = NULL;
	float **dct_Y = NULL,**dct_U = NULL,**dct_V = NULL;
	int ERROR;
	float value = 0;

	// unsigned char  test[8][8] = {{48,39,40,68,60,38,50,121} , {149,82,79,101,113,106,27,62} , {58,63,77,69,124,107,74,125} , {80,97,74,54,59,71,91,66} , {18,34,33,46,64,61,32,37} , {149,108,80,106,116,61,73,92} , {211,233,159,88,107,158,161,109} , {212,104,40,44,71,136,113,66} };
	// float output[8][8] = {0};
	// float value = 0;
	// int u,v,i,j,x,y;

	unsigned char luminanceQuantizationMatrix[8][8] = {
					{16,11,	10,	16,	24,	40,	51,	 61},
					{12,12,	14,	19,	26,	58,	60,	 55},
					{14,13,	16,	24,	40,	57,	69,	 56},
					{14,17,	22,	29,	51,	87,	80,	 62},
					{18,22,	37,	56,	68,	109,103,77},
					{24,35,	55,	64,	81,	104,113, 92},
					{49,64,	78,	87,	103,121,120,101},
					{72,92,	95,	98,	112,100,103, 99}
	};

	unsigned char chrominanceQuantizationMatrix[8][8] = {
					{17,18,	24,	47,	99,	99,	99,	99},
					{18,21,	26,	66,	99,	99,	99,	99},
					{24,26,	56,	99,	99,	99,	99,	99},
					{47,66,	99,	99,	99,	99,	99,	99},
					{99,99,	99,	99,	99,	99,	99,	99},
					{99,99,	99,	99,	99,	99,	99,	99},
					{99,99,	99,	99,	99,	99,	99,	99},
					{99,99,	99,	99,	99,	99,	99,	99}
	};

	if((image1 = readBMPfile(argv[1])) == NULL)
	{
		printf("Error in File 1\n");
		return -1;
	}

	red = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );
	blue = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );
	green = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );

	dct_red = (float **) malloc(image1->infoHeader.height * sizeof(float*) );
	dct_blue = (float **) malloc(image1->infoHeader.height * sizeof(float*) );
	dct_green = (float **) malloc(image1->infoHeader.height * sizeof(float*) );	

	Y = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );
	U = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );
	V = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );


	dct_Y = (float **) malloc(image1->infoHeader.height * sizeof(float*) );
	dct_U = (float **) malloc(image1->infoHeader.height * sizeof(float*) );
	dct_V = (float **) malloc(image1->infoHeader.height * sizeof(float*) );

	for (i = 0; i < image1->infoHeader.height; ++i)
	{
		red[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );
		blue[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );
		green[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );

		dct_red[i] = (float *) malloc(image1->infoHeader.width * sizeof(float ) );
		dct_blue[i] = (float *) malloc(image1->infoHeader.width * sizeof(float ) );
		dct_green[i] = (float *) malloc(image1->infoHeader.width * sizeof(float ) );
		
		Y[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );
		U[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );
		V[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );	

		dct_Y[i] = (float  *) malloc(image1->infoHeader.width * sizeof(float) );
		dct_U[i] = (float  *) malloc(image1->infoHeader.width * sizeof(float) );
		dct_V[i] = (float  *) malloc(image1->infoHeader.width * sizeof(float) );		
	}

	colourVector2MatrixConverter(red,green,blue,image1->bitMapImage,image1->infoHeader.height,image1->infoHeader.width);
	
	rgb2yuv(red,green,blue,Y,U,V,image1->infoHeader.height,image1->infoHeader.width);
	// for (u = 0; u < blockSize; ++u)
	// {
	// 	for (v = 0; v < blockSize; ++v)
	// 	{
			// value = 0;	
			// for (x = 0; x < blockSize; ++x)
			// {
			// 	for (y = 0; y < blockSize; ++y)
			// 	{
			// 		value += test[x][y] * cos(pi *(2*x + 1)*u/16) * cos(pi *(2*y + 1)*v/16)/4;
			// 	}
			// }
			// if (u == 0)
			// 	value *= 1/sqrt(2);
			// if(v == 0)
			// 	value *= 1/sqrt(2);
			// output[u][v] = value;
			
	// 	}
	// }


	// for (i = 0; i < image1->infoHeader.height; i += blockSize)
	// {
	// 	for (j = 0; j < image1->infoHeader.width; j += blockSize)
	// 	{
	// 		for (u = 0; u < blockSize; ++u)
	// 		{
	// 			for (v = 0; v < blockSize; ++v)
	// 			{
	// 				value = 0;	
	// 				for (x = 0; x < blockSize; ++x)
	// 				{
	// 					for (y = 0; y < blockSize; ++y)
	// 					{
	// 						value += red[x + i][y + j] * cos(pi *(2*x + 1)*u/16) * cos(pi *(2*y + 1)*v/16)/4;
	// 					}
	// 				}
	// 				if (u == 0)
	// 					value *= 1/sqrt(2);
	// 				if(v == 0)
	// 					value *= 1/sqrt(2);
	// 				dct_red[u + i][v + j] = value;
	// 			}
	// 		}

	// 	}
	// }

	// for (i = 0; i < image1->infoHeader.height; i += blockSize)
	// {
	// 	for (j = 0; j < image1->infoHeader.width; j += blockSize)
	// 	{
	// 		for (u = 0; u < blockSize; ++u)
	// 		{
	// 			for (v = 0; v < blockSize; ++v)
	// 			{
	// 				value = 0;	
	// 				for (x = 0; x < blockSize; ++x)
	// 				{
	// 					for (y = 0; y < blockSize; ++y)
	// 					{
	// 						value += green[x + i][y + j] * cos(pi *(2*x + 1)*u/16) * cos(pi *(2*y + 1)*v/16)/4;
	// 					}
	// 				}
	// 				if (u == 0)
	// 					value *= 1/sqrt(2);
	// 				if(v == 0)
	// 					value *= 1/sqrt(2);
	// 				dct_green[u + i][v + j] = value;
	// 			}
	// 		}

	// 	}
	// }

	// for (i = 0; i < image1->infoHeader.height; i += blockSize)
	// {
	// 	for (j = 0; j < image1->infoHeader.width; j += blockSize)
	// 	{
	// 		for (u = 0; u < blockSize; ++u)
	// 		{
	// 			for (v = 0; v < blockSize; ++v)
	// 			{
	// 				value = 0;	
	// 				for (x = 0; x < blockSize; ++x)
	// 				{
	// 					for (y = 0; y < blockSize; ++y)
	// 					{
	// 						value += blue[x + i][y + j] * cos(pi *(2*x + 1)*u/16) * cos(pi *(2*y + 1)*v/16)/4;
	// 					}
	// 				}
	// 				if (u == 0)
	// 					value *= 1/sqrt(2);
	// 				if(v == 0)
	// 					value *= 1/sqrt(2);
	// 				dct_blue[u + i][v + j] = value;
	// 			}
	// 		}

	// 	}
	// }
	

	for (i = 0; i < image1->infoHeader.height; i += blockSize)
	{
		for (j = 0; j < image1->infoHeader.width; j += blockSize)
		{
			for (u = 0; u < blockSize; ++u)
			{
				for (v = 0; v < blockSize; ++v)
				{
					value = 0;	
					for (x = 0; x < blockSize; ++x)
					{
						for (y = 0; y < blockSize; ++y)
						{
							value += Y[x + i][y + j] * cos(pi *(2*x + 1)*u/16) * cos(pi *(2*y + 1)*v/16)/4;
						}
					}
					if (u == 0)
						value *= 1/sqrt(2);
					if(v == 0)
						value *= 1/sqrt(2);
					dct_Y[u + i][v + j] = value / luminanceQuantizationMatrix[u][v];
				}
			}

		}
	}

	for (i = 0; i < image1->infoHeader.height; i += blockSize)
	{
		for (j = 0; j < image1->infoHeader.width; j += blockSize)
		{
			for (u = 0; u < blockSize; ++u)
			{
				for (v = 0; v < blockSize; ++v)
				{
					value = 0;	
					for (x = 0; x < blockSize; ++x)
					{
						for (y = 0; y < blockSize; ++y)
						{
							value += U[x + i][y + j] * cos(pi *(2*x + 1)*u/16) * cos(pi *(2*y + 1)*v/16)/4;
						}
					}
					if (u == 0)
						value *= 1/sqrt(2);
					if(v == 0)
						value *= 1/sqrt(2);
					dct_U[u + i][v + j] = value / chrominanceQuantizationMatrix[u][v];
				}
			}

		}
	}

	for (i = 0; i < image1->infoHeader.height; i += blockSize)
	{
		for (j = 0; j < image1->infoHeader.width; j += blockSize)
		{
			for (u = 0; u < blockSize; ++u)
			{
				for (v = 0; v < blockSize; ++v)
				{
					value = 0;	
					for (x = 0; x < blockSize; ++x)
					{
						for (y = 0; y < blockSize; ++y)
						{
							value += V[x + i][y + j] * cos(pi *(2*x + 1)*u/16) * cos(pi *(2*y + 1)*v/16)/4;
						}
					}
					if (u == 0)
						value *= 1/sqrt(2);
					if(v == 0)
						value *= 1/sqrt(2);
					dct_V[u + i][v + j] = value / chrominanceQuantizationMatrix[u][v];
				}
			}

		}
	}

/*

IDCT 

*/

	for (i = 0; i < image1->infoHeader.height; i += blockSize)
	{
		for (j = 0; j < image1->infoHeader.width; j += blockSize)
		{
			for (u = 0; u < blockSize; ++u)
			{
				for (v = 0; v < blockSize; ++v)
				{
					value = 0;	
					for (x = 0; x < blockSize; ++x)
					{
						for (y = 0; y < blockSize; ++y)
						{
							value += dct_Y[x + i][y + j] * cos(pi *(2*u + 1)*x/16) * cos(pi *(2*v + 1)*y/16)/4;
							if(x == 0)
								value *= 1/sqrt(2);
							if(y == 0)
								value *= 1/sqrt(2);							
						}
					}
					// if (u == 0)
					// 	value *= 1/sqrt(2);
					// if(v == 0)
					// 	value *= 1/sqrt(2);
					if(value > 255)
						Y[i][j] = 255;
					else if(value < 0)
						Y[i][j] = 0;
					else
						Y[i][j] = (unsigned char) value; 

				}
			}

		}
	}

	for (i = 0; i < image1->infoHeader.height; i += blockSize)
	{
		for (j = 0; j < image1->infoHeader.width; j += blockSize)
		{
			for (u = 0; u < blockSize; ++u)
			{
				for (v = 0; v < blockSize; ++v)
				{
					value = 0;	
					for (x = 0; x < blockSize; ++x)
					{
						for (y = 0; y < blockSize; ++y)
						{
							value += dct_U[x + i][y + j] * cos(pi *(2*u + 1)*x/16) * cos(pi *(2*v + 1)*y/16)/4;
							if(x == 0)
								value *= 1/sqrt(2);
							if(y == 0)
								value *= 1/sqrt(2);								
						}
					}
					// if (u == 0)
					// 	value *= 1/sqrt(2);
					// if(v == 0)
					// 	value *= 1/sqrt(2);
					if(value > 255)
						U[i][j] = 255;
					else if(value < 0)
						U[i][j] = 0;
					else
						U[i][j] = (unsigned char) value; 
				}
			}

		}
	}

	for (i = 0; i < image1->infoHeader.height; i += blockSize)
	{
		for (j = 0; j < image1->infoHeader.width; j += blockSize)
		{
			for (u = 0; u < blockSize; ++u)
			{
				for (v = 0; v < blockSize; ++v)
				{
					value = 0;	
					for (x = 0; x < blockSize; ++x)
					{
						for (y = 0; y < blockSize; ++y)
						{
							value += dct_V[x + i][y + j] * cos(pi *(2*u + 1)*x/16) * cos(pi *(2*v + 1)*y/16)/4;
							if(x == 0)
								value *= 1/sqrt(2);
							if(y == 0)
								value *= 1/sqrt(2);		
						}
					}
					// if (u == 0)
					// 	value *= 1/sqrt(2);
					// if(v == 0)
					// 	value *= 1/sqrt(2);
					if(value > 255)
						V[i][j] = 255;
					else if(value < 0)
						V[i][j] = 0;
					else
						V[i][j] = (unsigned char) value; 
				}
			}

		}
	}



	yuv2rgb(red,green,blue,Y,U,V,image1->infoHeader.width,image1->infoHeader.height);


	colourMatrix2VectorConverter(red,green,blue,image1->bitMapImage,image1->infoHeader.width,image1->infoHeader.height);
		
	if((image = fopen("dct_quantized.bmp","wb")) == NULL)
	{
		printf("Error Reading File\n");
		free(image1->bitMapImage);
		fclose(image);
		return -1;
	}

	if((ERROR = fwrite(&image1->header,sizeof(image1->header),1,image)) != 1)
	{
		printf("Error Writing Header\n");
		printf("ERROR: %d \n",ERROR);
		free(image1->bitMapImage);
		fclose(image);
		return -1;
	}

	if(fwrite(&image1->infoHeader,sizeof(image1->infoHeader),1,image) != 1)
	{
		printf("Error Writing InfoHeader\n");
		free(image1->bitMapImage);
		fclose(image);
		return -1;
	}

	fseek(image,image1->header.offset,SEEK_SET);
	if((ERROR = fwrite(image1->bitMapImage,image1->infoHeader.imagesize,1,image))  != 1) // Read the bit map as one data set
	{
		printf("Error Writing Image BitMap\n");
		printf("ERROR:%d\n",ERROR);
		free(image1->bitMapImage);
		fclose(image);
		return -1;
	}




	free(red);
	free(green);
	free(blue);
	free(Y);
	free(U);
	free(V)	;
	return 0;
}


// eliminate some dct coeifficients and calcuate psnr