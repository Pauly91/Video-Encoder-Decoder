#include <stdio.h>
#include <stdlib.h>
#include "bmpReader.h"


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

	char name[20];

	float **dct_Y = NULL,**dct_U = NULL,**dct_V = NULL;

	FILE *fp;

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

	for (i = 0; i < image->infoHeader.height; i++) // block of 2 * 2 
	{
		for (j = 0; j < image->infoHeader.width; j++) // block of 2 * 2 
		{
			
			printf("%d %d %d\n",Y[i][j],U[i][j],V[i][j]);
		}
	}

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


	return 0;
}