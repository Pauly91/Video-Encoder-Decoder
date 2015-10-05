#include <stdio.h>
#include <stdlib.h>
#include "bmpReader.h"

unsigned char luminanceQuantizationMatrixData[8][8] = {
				{16,11,	10,	16,	24,	40,	51,	 61},
				{12,12,	14,	19,	26,	58,	60,	 55},
				{14,13,	16,	24,	40,	57,	69,	 56},
				{14,17,	22,	29,	51,	87,	80,	 62},
				{18,22,	37,	56,	68,	109,103,77},
				{24,35,	55,	64,	81,	104,113, 92},
				{49,64,	78,	87,	103,121,120,101},
				{72,92,	95,	98,	112,100,103, 99}
};

unsigned char chrominanceQuantizationMatrixData[8][8] = {
				{17,18,	24,	47,	99,	99,	99,	99},
				{18,21,	26,	66,	99,	99,	99,	99},
				{24,26,	56,	99,	99,	99,	99,	99},
				{47,66,	99,	99,	99,	99,	99,	99},
				{99,99,	99,	99,	99,	99,	99,	99},
				{99,99,	99,	99,	99,	99,	99,	99},
				{99,99,	99,	99,	99,	99,	99,	99},
				{99,99,	99,	99,	99,	99,	99,	99}
};

int main(int argc, char const *argv[])
{
	if(argc != 4)
	{
		printf("\n ./run [file name] [blockSize] [type of downSampling - 444 422 411 421] \n\n");
		return -1;
	}
	BMPData *image = NULL;

	int i,j,u,v,x,y;
	int blockSize = atoi(argv[2]);
	char question;
	if(blockSize != 8)
	{
		printf("Only 8*8 quantization block exist\n");
		printf("Continue with block size of 8*8 ? Y or N \n");
		//getc(question); // complete this question
	}
	int dSampleU = 0;
	int dSampleV = 0;
	int DsampleType = 0	;

	DsampleType = atoi(argv[3]);
	dSampleV = DsampleType%10;
	DsampleType /= 10;
	dSampleU = DsampleType%10;
	printf("dSampleU:%d dSampleV:%d\n",dSampleU,dSampleV);		

	unsigned char **red,**green,**blue;
	unsigned char **Y,**U,**V;
	unsigned char **luminanceQuantizationMatrix;
	unsigned char **chrominanceQuantizationMatrix;

	luminanceQuantizationMatrix = (unsigned char **) malloc(blockSize * sizeof(char*) );
	chrominanceQuantizationMatrix = (unsigned char **) malloc(blockSize * sizeof(char*) );

	// unsigned char luminanceQuantizationMatrixData[8][8] = {
	// 				{16,11,	10,	16,	24,	40,	51,	 61},
	// 				{12,12,	14,	19,	26,	58,	60,	 55},
	// 				{14,13,	16,	24,	40,	57,	69,	 56},
	// 				{14,17,	22,	29,	51,	87,	80,	 62},
	// 				{18,22,	37,	56,	68,	109,103,77},
	// 				{24,35,	55,	64,	81,	104,113, 92},
	// 				{49,64,	78,	87,	103,121,120,101},
	// 				{72,92,	95,	98,	112,100,103, 99}
	// };

	// unsigned char chrominanceQuantizationMatrixData[8][8] = {
	// 				{17,18,	24,	47,	99,	99,	99,	99},
	// 				{18,21,	26,	66,	99,	99,	99,	99},
	// 				{24,26,	56,	99,	99,	99,	99,	99},
	// 				{47,66,	99,	99,	99,	99,	99,	99},
	// 				{99,99,	99,	99,	99,	99,	99,	99},
	// 				{99,99,	99,	99,	99,	99,	99,	99},
	// 				{99,99,	99,	99,	99,	99,	99,	99},
	// 				{99,99,	99,	99,	99,	99,	99,	99}
	// };

	char name[20];

	float **dct_Y = NULL,**dct_U = NULL,**dct_V = NULL;
	int **dct_Y_int = NULL,**dct_U_int = NULL,**dct_V_int = NULL;
	FILE *fp;

	for (i = 0; i < blockSize; ++i)
	{
		luminanceQuantizationMatrix [i] = (unsigned char *) malloc(blockSize * sizeof(char) );
		chrominanceQuantizationMatrix[i] = (unsigned char *) malloc(blockSize * sizeof(char) );
	}	
	for (i = 0; i < blockSize; ++i)
	{
		for (j = 0; j < blockSize; ++j)
		{
			luminanceQuantizationMatrix[i][j] = luminanceQuantizationMatrixData[i][j];
			chrominanceQuantizationMatrix[i][j] = chrominanceQuantizationMatrixData[i][j];
		}
	}

	if((image = readBMPfile(argv[1])) == NULL)
		printf("Error in File 1\n");	

	red = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );
	blue = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );
	green = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );

	Y = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );
	U = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );
	V = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );

	dct_Y = (float **) malloc(image->infoHeader.height * sizeof(float*) );
	dct_U = (float **) malloc(image->infoHeader.height * sizeof(float*) );
	dct_V = (float **) malloc(image->infoHeader.height * sizeof(float*) );

	dct_Y_int = (int **) malloc(image->infoHeader.height * sizeof(int*) );
	dct_U_int = (int **) malloc(image->infoHeader.height * sizeof(int*) );
	dct_V_int = (int **) malloc(image->infoHeader.height * sizeof(int*) );

	for (i = 0; i < image->infoHeader.height; ++i)
	{
		red[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );
		blue[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );
		green[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );

		Y[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );
		U[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );
		V[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );	

		dct_Y[i] = (float  *) malloc(image->infoHeader.width * sizeof(float) );
		dct_U[i] = (float  *) malloc(image->infoHeader.width * sizeof(float) );
		dct_V[i] = (float  *) malloc(image->infoHeader.width * sizeof(float) );	

		dct_Y_int[i] = (int  *) malloc(image->infoHeader.width * sizeof(int) );
		dct_U_int[i] = (int  *) malloc(image->infoHeader.width * sizeof(int) );
		dct_V_int[i] = (int  *) malloc(image->infoHeader.width * sizeof(int) );	

	}

	colourVector2MatrixConverter(red,green,blue,image->bitMapImage,image->infoHeader.height,image->infoHeader.width);
	
	rgb2yuv(red,green,blue,Y,U,V,image->infoHeader.height,image->infoHeader.width);



// Reading and writing as characters

// Write the YUV file

////////////////////////////////////////////////////////////////////////
	if((fp = fopen("YUV","w")) == NULL)
	{
		printf("YUV file not opened\n");
		return -1;
	}

	for (i = 0; i < image->infoHeader.height; ++i)
	{
		for (j = 0; j < image->infoHeader.width; ++j)
		{
			fprintf(fp, "%c%c%c",Y[i][j],U[i][j],V[i][j]);
		}
	}
	fclose(fp);
////////////////////////////////////////////////////////////////////////


// Read the YUV file

////////////////////////////////////////////////////////////////////////	

	if((fp = fopen("YUV","r")) == NULL)
	{
		printf("YUV file not opened\n");
		return -1;
	}

	for (i = 0; i < image->infoHeader.height; ++i)
	{
		for (j = 0; j < image->infoHeader.width; ++j)
		{
			fscanf(fp, "%c%c%c",&Y[i][j],&U[i][j],&V[i][j]);
		}
	}
	fclose(fp);

////////////////////////////////////////////////////////////////////////

	DCT(image, dct_Y, dct_U, dct_V, Y, U, V, blockSize);

////////////////////////////////////////////////////////////////////////

	for (i = 0,x = 1; i < image->infoHeader.height; i += blockSize)
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize)
		{
			sprintf(name, "dct/YBlock%d",x);
			if((fp = fopen(name,"w")) == NULL)
			{
				printf("%s not opened\n",name);
				return -1;
			}
			for (u = 0; u < blockSize; ++u)
			{
				for (v = 0; v < blockSize; ++v)
				{
					fprintf(fp, "%f ",dct_Y[i + u][j + v]);
				}
				fprintf(fp, "\n");
			}
			fclose(fp);

			sprintf(name, "dct/UBlock%d",x);
			if((fp = fopen(name,"w")) == NULL)
			{
				printf("%s not opened\n",name);
				return -1;
			}
			for (u = 0; u < blockSize; ++u)
			{
				for (v = 0; v < blockSize; ++v)
				{
					fprintf(fp, "%f ",dct_U[i + u][j + v]);
				}
				fprintf(fp, "\n");
			}
			fclose(fp);

			sprintf(name, "dct/VBlock%d",x);
			if((fp = fopen(name,"w")) == NULL)
			{
				printf("%s not opened\n",name);
				return -1;
			}
			for (u = 0; u < blockSize; ++u)
			{
				for (v = 0; v < blockSize; ++v)
				{
					fprintf(fp, "%f ",dct_V[i + u][j + v]);
				}
				fprintf(fp, "\n");
			}
			fclose(fp);
		}
	}


////////////////////////////////////////////////////////////////////////

	downSample(image, Y, U, V, dSampleU, dSampleV); // CHeck the loop in the definition of this function

	// for (i = 0; i < image->infoHeader.height; i++) // block of 2 * 2 
	// {
	// 	for (j = 0; j < image->infoHeader.width; j++) // block of 2 * 2 
	// 	{
			
	// 		printf("%d %d %d\n",Y[i][j],U[i][j],V[i][j]);
	// 	}
	// }

////////////////////////////////////////////////////////////////////////


	if((fp = fopen("downSample","w")) == NULL)
	{
		printf("downSample file not opened\n");
		return -1;
	}

	for (i = 0; i < image->infoHeader.height; i += 2) // block of 2 * 2 
	{
		for (j = 0; j < image->infoHeader.width; j += 2) // block of 2 * 2 
		{
			
			for (x = 0; x < 2; ++x)
			{
				for (y = 0; y < 2; ++y)
				{
					fprintf(fp, "%d ",Y[i + x][j + y]);				
				}
			}
			fprintf(fp, "%d %d\n",U[i][j],V[i][j]);
		}
	}
	fclose(fp);
////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////

/*

Read from the file downSample

*/

////////////////////////////////////////////////////////////////////////

	DCT(image, dct_Y, dct_U, dct_V, Y, U, V, blockSize);

////////////////////////////////////////////////////////////////////////

	if((fp = fopen("Dct","w")) == NULL)
	{
		printf("dct file not opened\n");
		return -1;
	}

	for (i = 0; i < image->infoHeader.height; ++i) // block of 2 * 2 
	{
		for (j = 0; j < image->infoHeader.width; ++j) // block of 2 * 2 
		{
			fprintf(fp, "%f %f %f\n",dct_Y[i][j],dct_U[i][j],dct_V[i][j]);
		}
	}
	fclose(fp);

////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////

/*

Read from the file Dct

*/

////////////////////////////////////////////////////////////////////////

	quantize(image,dct_Y, dct_U, dct_V, luminanceQuantizationMatrix, chrominanceQuantizationMatrix, blockSize);

	for (i = 0; i < image->infoHeader.height; ++i) // block of 2 * 2 
	{
		for (j = 0; j < image->infoHeader.width; ++j) // block of 2 * 2 
		{
			dct_V_int[i][j] = dct_V[i][j];
			dct_U_int[i][j] = dct_U[i][j];
			dct_Y_int[i][j] = dct_U[i][j];
		}
	}
////////////////////////////////////////////////////////////////////////

	if((fp = fopen("Dct_quantized","w")) == NULL)
	{
		printf("Dct_quantized file not opened\n");
		return -1;
	}

	for (i = 0; i < image->infoHeader.height; ++i) // block of 2 * 2 
	{
		for (j = 0; j < image->infoHeader.width; ++j) // block of 2 * 2 
		{
			fprintf(fp, "%f %f %f\n",dct_Y[i][j],dct_U[i][j],dct_V[i][j]);
		}
	}
	fclose(fp);

////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////

/*

Read from the file dct

*/

////////////////////////////////////////////////////////////////////////

zigzag(image, dct_Y_int, blockSize,"zigzag_DCT_Y");
zigzag(image, dct_U_int, blockSize,"zigzag_DCT_U");
zigzag(image, dct_V_int, blockSize,"zigzag_DCT_V");



	return 0;
}