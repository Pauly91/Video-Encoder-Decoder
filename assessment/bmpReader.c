#include <stdio.h>
#include <stdlib.h>
// #include <math.h>	

#include "bmpReader.h"


BMPData* readBMPfile(const char *filename)
{
	BMPData *bmpData;
	bmpData = (BMPData*) malloc(sizeof(BMPData));
	FILE *fp;
	int ERROR;
	if((fp = fopen(filename ,"rb")) == NULL)
	{
		printf("Error Reading File\n");
		fclose(fp);
		return NULL;
	}

	if(fread(&bmpData->header,sizeof(bmpData->header),1,fp) != 1)
	{
		printf("Error Reading Header\n");
		fclose(fp);
		return NULL;
	}

	if (bmpData->header.type != 19778)
	{
		printf("Not a BMP file\n");
		fclose(fp);
		return NULL;
	}
	// printf("**Header**\n");
	// printf(" type: 0x%x \n fileSize: %d \n reserved1: %d \n offset: %d \n",header.type,header.fileSize,header.reserved,header.offset);

	if(fread(&bmpData->infoHeader,sizeof(bmpData->infoHeader),1,fp) != 1)
	{
		printf("Error Reading Header\n");
		fclose(fp);
		return NULL;
	}
	// printf("\n\n**InfoHeader**\n");
	//printf(" size: %d \n width: %d \n height: %d \n planes: %d \n bits: %d \n",bmpData->infoHeader.size,bmpData->infoHeader.width,bmpData->infoHeader.height,bmpData->infoHeader.planes,bmpData->infoHeader.bits); 
	// printf(" compression: %d \n imagesize: %d \n xResolution: %d \n yResolution: %d \n",infoHeader.compression,infoHeader.imagesize,infoHeader.xResolution,infoHeader.yResolution);
	// printf(" coloursUsed: %d \n importantColours: %d \n\n",infoHeader.coloursUsed,infoHeader.importantColours);

	bmpData->bitMapImage = (unsigned char *) malloc(bmpData->infoHeader.imagesize * sizeof(char));

	fseek(fp,bmpData->header.offset,SEEK_SET);
	if((ERROR = fread(bmpData->bitMapImage,bmpData->infoHeader.imagesize,1,fp))  != 1) 
	{
		printf("Error Reading Image BitMap\n");
		printf("ERROR:%d\n",ERROR);
		free(bmpData->bitMapImage);
		fclose(fp);
		return NULL;
	}
	fclose(fp);
	return bmpData;
}

int writeBMPfile(BMPData *data,const char *filename)
{

	int ERROR;
	FILE *image;
	if((image = fopen(filename,"wb")) == NULL)
	{
		printf("Error Reading File\n");
		fclose(image);
		return -1;
	}

	if((ERROR = fwrite(&(data->header),sizeof(data->header),1,image)) != 1)
	{
		printf("Error Writing Header\n");
		printf("ERROR: %d \n",ERROR);
		fclose(image);
		return -1;
	}

	if(fwrite(&(data->infoHeader),sizeof(data->infoHeader),1,image) != 1)
	{
		printf("Error Writing InfoHeader\n");
		fclose(image);
		return -1;
	}

	fseek(image,data->header.offset,SEEK_SET);
	if((ERROR = fwrite((data->bitMapImage),data->infoHeader.imagesize,1,image))  != 1) // check if sizeof is needed ?
	{
		printf("Error Writing Image BitMap\n");
		printf("ERROR:%d\n",ERROR);
		fclose(image);
		return -1;
	}
	fclose(image);
	return 1;
}


void colourVector2MatrixConverter(unsigned char ** red,unsigned char ** green,unsigned char ** blue,unsigned char * bitMapImage,int height, int width)
{
	int i,j,k;
	for (i = 0,j = 0, k = 0; i < height * width * numColor; i+=3)
	{
			blue[j][k] = bitMapImage[i]; 
			green[j][k] = bitMapImage[i + 1];
			red[j][k] = bitMapImage[i + 2];
	 		// printf(" %u j:%d k:%d\n",red[j][k],j,k);
			k++;
			if(k == width)
			{
				j++;
				k = 0;
			}
		
	}
}

void rgb2yuv(unsigned char ** red,unsigned char ** green,unsigned char ** blue, unsigned char ** Y,unsigned char ** U,unsigned char ** V,int height, int width)
{
	int i,j;
	int value;
	for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; ++j)
		{
			value = red[i][j] * 0.2999 + green[i][j] * 0.587 + blue[i][j] * 0.114;  
			if(value > 255)
				Y[i][j] = 255;
			else if(value < 0)
				Y[i][j] = 0;
			else
				Y[i][j] = (unsigned char) value; 

			value = red[i][j] * (-0.168736) + green[i][j] * (-0.331264) + blue[i][j] * (0.500002) + 128;
			if(value > 255)
				U[i][j] = 255;
			else if(value < 0)
				U[i][j] = 0;
			else
				U[i][j] = (unsigned char) value; 

			value = red[i][j] * 0.5 + green[i][j] * (-0.418688) + blue[i][j] * (-0.081312) + 128;
			if(value > 255)
				V[i][j] = 255;
			else if(value < 0)
				V[i][j] = 0;
			else
				V[i][j] = (unsigned char) value; 			
		}
	}

}


void yuv2rgb(unsigned char ** red,unsigned char ** green,unsigned char ** blue, unsigned char ** Y,unsigned char ** U,unsigned char ** V,int height, int width)
{	
	int i,j;
	int value;
	for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; ++j)
		{
			value = Y[i][j] + (V[i][j] - 128 )* 1.4021;  
			if(value > 255)
				red[i][j] = 255;
			else if(value < 0)
				red[i][j] = 0;
			else
				red[i][j] = (unsigned char) value;

			value = Y[i][j] + (U[i][j] - 128 ) * (-0.34414) + (V[i][j] - 128) * (-0.71414);  
			if(value > 255)
				green[i][j] = 255;
			else if(value < 0)
				green[i][j] = 0;
			else
				green[i][j] = (unsigned char) value;

			value = Y[i][j] + (U[i][j] - 128) * 1.77180;
			if(value > 255)
				blue[i][j] = 255;
			else if(value < 0)
				blue[i][j] = 0;
			else
				blue[i][j] = (unsigned char) value;

		}
	}

}

void colourMatrix2VectorConverter(unsigned char ** red,unsigned char ** green,unsigned char ** blue,unsigned char * bitMapImage,int height, int width)
{
	int i,j,k;
	for (i = 0,j = 0, k = 0; i < height * width * numColor; i+=3)
	{
			bitMapImage[i] = blue[j][k]; 
			bitMapImage[i + 1] = green[j][k]; 
			bitMapImage[i + 2] = red[j][k]; 
			k++;
			if(k == width)
			{
				j++;
				k = 0;
			}
		
	}
}


void DCT(BMPData *image, float **dct_Y, float **dct_U, float **dct_V, unsigned char ** Y, unsigned char ** U, unsigned char ** V, int blockSize)
{

	int i,j,u,v,x,y;
	float value = 0;
	for (i = 0; i < image->infoHeader.height; i += blockSize)
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize)
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
					dct_Y[u + i][v + j] = value;// / luminanceQuantizationMatrix[u][v];
				}
			}

		}
	}

	for (i = 0; i < image->infoHeader.height; i += blockSize)
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize)
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
					dct_U[u + i][v + j] = value;// / chrominanceQuantizationMatrix[u][v];
				}
			}

		}
	}

	for (i = 0; i < image->infoHeader.height; i += blockSize)
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize)
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
					dct_V[u + i][v + j] = value;// / chrominanceQuantizationMatrix[u][v];
				}
			}

		}
	}

}


void downSample(BMPData *image, unsigned char ** Y, unsigned char ** U, unsigned char ** V, int dSampleU, int dSampleV)
{

	int i,j,k,l;
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
				dSampleU = 0;
				dSampleV = 2;
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
						 	U[i + l][j + k] = U[i][j];
						 	V[i + l][j + k] = V[i][j];
					 	}
					}

				}
			}		
		}

	}	
}


void quantize(BMPData *image, float **dct_Y, float **dct_U, float **dct_V, unsigned char ** luminanceQuantizationMatrix, unsigned char ** chrominanceQuantizationMatrix, int blockSize)
{

	int i,j,u,v;
	for (i = 0; i < image->infoHeader.height; i += blockSize)
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize)
		{
			for (u = 0; u < blockSize; ++u)
			{
				for (v = 0; v < blockSize; ++v)
				{
					dct_Y[u + i][v + j] /= luminanceQuantizationMatrix[u][v];
				}
			}

		}
	}

	for (i = 0; i < image->infoHeader.height; i += blockSize)
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize)
		{
			for (u = 0; u < blockSize; ++u)
			{
				for (v = 0; v < blockSize; ++v)
				{
					dct_U[u + i][v + j] /= chrominanceQuantizationMatrix[u][v];
				}
			}

		}
	}

	for (i = 0; i < image->infoHeader.height; i += blockSize)
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize)
		{
			for (u = 0; u < blockSize; ++u)
			{
				for (v = 0; v < blockSize; ++v)
				{
					dct_V[u + i][v + j] /= chrominanceQuantizationMatrix[u][v];
				}
			}

		}
	}	


}

void zigzag(BMPData *image, int **dct,int blockSize, char *filename)
{
	int i,j,m,n;
	int holder;
	FILE *fp;
	printf("%s\n",filename);
	if((fp = fopen(filename,"w")) == NULL)
	{
		printf("%s not open\n",filename);
		return ;
	}

	for (m = 0; m < image->infoHeader.height; m += blockSize)
	{
		for (n = 0; n < image->infoHeader.width; n += blockSize)
		{
			i = 0;
			j = 0;
			while( i != blockSize && j != blockSize)
			{

				if(j%2 == 0 && i == 0)
				{
					fprintf(fp,"%d ",dct[m + i][n + j]);
					j++;
				}
				else if(j%2 == 1 && i == 0)
				{
					while(j != 0)
					{
						fprintf(fp,"%d ",dct[m + i][n + j]);
						j -= 1;
						i += 1;
					}
				}
				else if(i%2 == 0 && j == 0)
				{
					while(i != 0)
					{
						fprintf(fp,"%d ",dct[m + i][n + j]);
						j += 1;
						i -= 1;
					}			
				}
				else if(i%2 == 1 && j == 0)
				{
					if(i == blockSize - 1)
					{
						
						fprintf(fp,"%d ",dct[m + i][n + j]);
						j++;
					}

					else
					{
						fprintf(fp,"%d ",dct[m + i][n + j]);
						i++;
					}

				}
				else if(i == blockSize - 1 && j%2 == 1)
				{
					holder = j;
					while(i != holder)
					{
						fprintf(fp,"%d ",dct[m + i][n + j]);
						j += 1;
						i -= 1;				
					}
				}
				else if(i == blockSize - 1 && j%2 == 0)
				{
						fprintf(fp,"%d ",dct[m + i][n + j]);
						j++;
				}		
				else if(j == blockSize - 1 && i%2 == 1)
				{
					fprintf(fp,"%d ",dct[m + i][n + j]);
					i++;
				}
				else if(j == blockSize - 1 && i%2 == 0)
				{
					holder = i;
					while(j != holder)
					{
						fprintf(fp,"%d ",dct[m + i][n + j]);
						j -= 1;
						i += 1;				
					}
				}
				if(i == blockSize -1 && j == blockSize - 1)
				{
						fprintf(fp,"%d ",dct[m + i][n + j]);
						i++;
						j++;
						break;
				}

			}
			fprintf(fp,"\n");
		}

	}
	fclose(fp);	
}