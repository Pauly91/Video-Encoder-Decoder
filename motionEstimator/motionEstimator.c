#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include  <math.h>
#include "bmpReader.h"


struct MotionVector
{
	int x;
	int y;
};

int main(int argc, char *argv[])
{
	if(argc != 5)
	{
		printf("\n ./run [file name1] [file name2] [depth of search] [blockSize] \n\n");
		return -1;
	}
	BMPData *image1 = NULL,*image2 = NULL;
	int depth = atoi(argv[3]);
	int blockSize = atoi(argv[4]);
	int i,j,p,q,r;
	int Nbhors;
	int *pathX;
	int *pathY;
	int newX,newY;
	int motionVectorCount,count;
	struct MotionVector * motionVector;

	if(depth == 0)
		Nbhors = 0;
	else
		Nbhors = pow(3,1+depth);

	pathY = (int *) malloc(Nbhors * sizeof(int));
	pathX = (int *) malloc(Nbhors * sizeof(int));



	for (i = -depth,count = 0; i <= depth; ++i)
	{
		for (j = -depth; j <= depth; ++j)
		{
			pathX[count] = i;
			pathY[count] = j;
			++count;
		}
	}

	unsigned char **red = NULL,**green = NULL,**blue = NULL;
	unsigned char **Y1 = NULL,**U1 = NULL,**V1 = NULL;	
	unsigned char **Y2 = NULL,**U2 = NULL,**V2 = NULL;

	float MAD,previousMAD;


	if((image1 = readBMPfile(argv[1])) == NULL)
	{
		printf("Error reading %s\n",argv[1]);	
		return -1;
	}
	if((image2 = readBMPfile(argv[2])) == NULL)
	{
		printf("Error reading %s\n",argv[2]);	
		return -1;
	}	
	
	if((image1->infoHeader.height != image2->infoHeader.height) || (image1->infoHeader.width != image2->infoHeader.width))
	{
		printf("Images are not of the same Size !! \n");
		return -1;
	}

	motionVector = (struct MotionVector *) malloc((image1->infoHeader.height/blockSize) * (image1->infoHeader.width/blockSize) * sizeof(struct MotionVector)); 	


	red = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );
	blue = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );
	green = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );

	Y1 = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );
	U1 = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );
	V1 = (unsigned char **) malloc(image1->infoHeader.height * sizeof(char*) );

	Y2 = (unsigned char **) malloc(image2->infoHeader.height * sizeof(char*) );
	U2 = (unsigned char **) malloc(image2->infoHeader.height * sizeof(char*) );
	V2 = (unsigned char **) malloc(image2->infoHeader.height * sizeof(char*) );

	for (i = 0; i < image1->infoHeader.height; ++i)
	{
		red[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );
		blue[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );
		green[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );

		Y1[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );
		U1[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );
		V1[i] = (unsigned char *) malloc(image1->infoHeader.width * sizeof(char) );	
		
		Y2[i] = (unsigned char *) malloc(image2->infoHeader.width * sizeof(char) );
		U2[i] = (unsigned char *) malloc(image2->infoHeader.width * sizeof(char) );
		V2[i] = (unsigned char *) malloc(image2->infoHeader.width * sizeof(char) );	

	}
	colourVector2MatrixConverter(red,green,blue,image1->bitMapImage,image1->infoHeader.height,image1->infoHeader.width);
	rgb2yuv(red,green,blue,Y1,U1,V1,image1->infoHeader.height,image1->infoHeader.width);
	colourVector2MatrixConverter(red,green,blue,image2->bitMapImage,image2->infoHeader.height,image2->infoHeader.width);
	rgb2yuv(red,green,blue,Y2,U2,V2,image2->infoHeader.height,image2->infoHeader.width);	

	for (i = 0, motionVectorCount = 0; i < image2->infoHeader.height; i += blockSize)
	{
		for (j = 0; j < image2->infoHeader.width; j += blockSize)
		{
			previousMAD = MAD = 0;
			for (r = 0; r < Nbhors; ++r)
			{		
				for (p = 0; p < blockSize; ++p)
				{
					for (q = 0; q < blockSize; ++q)
					{
						newX = i + p + pathX[r];
						newY = j + q + pathY[r];
						if(newX < 0 || newX > image2->infoHeader.height - 1 || newY < 0 || newY > image2->infoHeader.width - 1)
							continue;
						MAD += abs(Y1[i + p][j + q] - Y2[newX][newY]) + abs(U1[i + p][j + q] - U2[newX][newY]) + abs(V1[i + p][j + q] - V2[newX][newY]);  
					}
				}
				MAD = MAD/(blockSize * blockSize);
				printf("MAD:%f\n",MAD);
				if(MAD > previousMAD)
				{
					motionVector[motionVectorCount].x = pathX[r];
					motionVector[motionVectorCount].y = pathY[r];
					previousMAD = MAD;

				}	
			}
			if(MAD < 20)
			{
				motionVector[motionVectorCount].x = 0;
				motionVector[motionVectorCount].y = 0;
			}
			if(motionVector[motionVectorCount].x != 0 && motionVector[motionVectorCount].y != 0)
			{
				for (p = 0; p < blockSize; ++p)
				{
					for (q = 0; q < blockSize; ++q)
					{			
						newX = i + p + motionVector[motionVectorCount].x;
						newY = j + q + motionVector[motionVectorCount].y;
						if(newX < 0 || newX > image2->infoHeader.height - 1 || newY < 0 || newY > image2->infoHeader.width - 1)
							continue;

						Y2[newX][newY] = 255;
						U2[newX][newY] = 0;
						V2[newX][newY] = 0;
					}
				}	
			}
			motionVectorCount++;
		}	

	}
	for (i = 0; i < motionVectorCount; ++i)
	{
		printf("%d %d\n",motionVector[i].x,motionVector[i].y);
	}

	yuv2rgb(red, green, blue, Y2, U2, V2, image2->infoHeader.height, image2->infoHeader.width);
	colourMatrix2VectorConverter(red,green,blue,image2->bitMapImage,image2->infoHeader.height,image2->infoHeader.width);
	if(writeBMPfile(image2,"motionEstimated.bmp") < 0)
	{
		printf("Error Writing to motionEstimated file\n");
		return -1;
	}		

	return 0;
}