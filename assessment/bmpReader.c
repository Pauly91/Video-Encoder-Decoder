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


unsigned char mask = 128;

int previousDC = 0;
char firstFlag = 1;

struct HuffmanDCTable
{
	int size;
	int numberOfBits;
	char *bitpattern;
}; 
struct HuffmanACTable
{
	int run;
	int size;
	int numberOfBits;
	char *bitpattern;
}; 

int bitCount(int value)
{
	int count = 0;
	value = abs(value);
	//printf("bitCount value: %d\n",value);
	while(value)
	{
		value >>= 1;
		count++;
	}
	return count;
}
void binaryDisplay(int *code,int size)
{
	int i;
	for (i = 0; i < size; ++i)
	{
		printf("%d",code[i]);
	}
}
void getBinary(int number,int* code, int size)
{
	int j = size,i;
	int holder = number;
	// printf("holder:%d \n",holder);
	//char *bitpattern;
	number = abs(number);
	size--;
	while(number)
	{
		if(number%2 == 0)
		{
			code[size] = 0;;
		}
		else if(number%2 == 1)
		{
			code[size] = 1;
		}
		size--;
		number >>= 1;
	}
	// for (i = 0; i < j; ++i)
	// {
	// 	printf("%d\n",code[j]);
	// }
	// printf("\n");
	// printf("holder:%d \n",holder);
	if(holder < 0)
	{
		for (i = 0; i < j; ++i)
		{
			if(code[i] == 1)
				code[i] = 0;
			else 
				code[i] = 1;

		}
	}
	// for (i = 0; i < j; ++i)
	// {
	// 	printf("%d\n",code[j]);
	// }
	// printf("\n");
}

void writeTofile(FILE *fp,char *bitpattern,int sizeOfBitPattern, int *code, int sizeOfCode)
{
	int i;
	for (i = 0; i < sizeOfBitPattern; ++i)
	{
		// printf("%c",bitpattern[i]);
		fprintf(fp,"%c",bitpattern[i]);
	}
	// fprintf(fp,"%s",bitpattern);	
	for (i = 0; i < sizeOfCode; ++i)
	{
		// printf("%d",code[i]);
		fprintf(fp,"%d",code[i]);
	}
	//fprintf(fp,"\n");	
	// printf("\n");	

}



void dpcm(char DC, struct HuffmanDCTable *huffmanDCTable, FILE *encodedData)
{
	int *code;
	int size;
	int j;
	//char *bitpattern;
	if(firstFlag == 1)
	{
		previousDC = DC;
		firstFlag = 0;
	}
	else
		DC -= previousDC;
	size = bitCount(DC);
	// printf("size:%d\n",size);
	code = (int *) malloc(size*sizeof(int));
	getBinary(DC,code,size);
	// printf("\nNumber:%d Size:%d Binary:",DC,size);
	// //binaryDisplay(code,size);
	// printf("\n");
	for (j = 0; j < DCtableSize; ++j)
	{
		if(huffmanDCTable[j].size == size)
		{
			// bitpattern = (char *) malloc(huffmanDCTable[j].numberOfBits * sizeof(char));
			// bitpattern = huffmanDCTable[j].bitpattern;
			writeTofile(encodedData,huffmanDCTable[j].bitpattern,huffmanDCTable[j].numberOfBits,code,size);
			break;
		}
	}
	free(code);	

}

void rlcEncode(int count,int data,struct HuffmanACTable *huffmanACTable,FILE *encodedData)
{
	int i;
	//printf("data:%d\n",data);
	int size = bitCount(data);
	// if(size == 1)
	// 	size++;

	int *code;
	code = (int *) malloc(size*sizeof(int));
	getBinary(data,code,size);	
	for (i = 0; i < ACtableSize; ++i)
	{
		//printf("data:%d size:%d count:%d Hsize:%d Hrun:%d code: %s\n",data,size,count,huffmanACTable[i].size,huffmanACTable[i].run,huffmanACTable[i].bitpattern);
		if(huffmanACTable[i].size == size && huffmanACTable[i].run == count)
		{
			//printf("numberOfBits:%d\n",huffmanACTable[i].numberOfBits);
			//printf("rle code: %s\n",huffmanACTable[i].bitpattern);
			// for (j = 0; i < huffmanACTable[i].numberOfBits; ++j)
			// {
			// 	/* code */
			// }
			//rleBitpattern = huffmanACTable[i].bitpattern;
			writeTofile(encodedData,huffmanACTable[i].bitpattern,huffmanACTable[i].numberOfBits,code,size);	

			break;
		}
	}	
}

void rlc(char *dataVector,struct HuffmanACTable *huffmanACTable,FILE *encodedData)
{

	int i,count = 0;
	FILE *fp = NULL;
	if(!(fp = fopen("rle","a+")))
	{
		printf("rle not opened\n");
		return;
	}	
	//printf("\n");
	for (i = 1; i < dataSize; ++i)
	{
		if(dataVector[i] == 0)
		{
			while(dataVector[i] == 0)
			{
				i++;
				count++;
			}
			if(i > dataSize-1)
			{
				fprintf(fp,"%d %d\n",count,0);
				fprintf(encodedData,"1010");
				//rlcEncode(count,0,huffmanACTable,encodedData);
			}
			else
			{
				fprintf(fp,"%d %d\n",count,dataVector[i]);
				rlcEncode(count,dataVector[i],huffmanACTable,encodedData);
			}
			count = 0;
		}
		else
		{
			fprintf(fp,"%d %d\n",count,dataVector[i]);
			rlcEncode(count,dataVector[i],huffmanACTable,encodedData);
		}
	}
	fclose(fp);
}

void entropyCoding(char *dataVector,struct HuffmanDCTable *huffmanDCTable,struct HuffmanACTable *huffmanACTable,FILE *encodedData)
{

	dpcm(dataVector[0],huffmanDCTable,encodedData);
	rlc(dataVector,huffmanACTable,encodedData);
}


// Check for error
void bitPacking(char * fileNameSource, char * fileNameTarget)
{
	char value;
	unsigned char byte = 0;
	unsigned char runTimeMask = mask;
	FILE *fp = NULL,*target = NULL;
	if(!(fp = fopen(fileNameSource,"r")))
	{
		printf("%s not open\n",fileNameSource);
		return ;
	}
	if(!(target = fopen(fileNameTarget,"w")))
	{
		printf("%s not open\n",fileNameTarget);
		return ;
	}
	value = fgetc(fp);
	while(!feof(fp))
	{
		//printf("%c",value);
		if(value == '1')
		{
			//printf("Value:%c\n",value);
			byte |= runTimeMask;
		}
		//printf("%d\n",runTimeMask);
		runTimeMask >>= 1;

		if(runTimeMask == 0)
		{
			//printf("%d\n",byte);
			fprintf(target,"%c",byte);
			byte = 0;
			runTimeMask = mask;
		}
		value = fgetc(fp);
	} 
	if(runTimeMask != 0)
	{
		fprintf(target,"%c",byte);
	}
	fclose(target);
	fclose(fp);


}


// int main(int argc, char const *argv[])
// {
	
// 	struct HuffmanDCTable *huffmanDCTable = NULL;
// 	huffmanDCTable = (struct HuffmanDCTable *) malloc(sizeof(struct HuffmanDCTable) * DCtableSize);
// 	struct HuffmanACTable *huffmanACTable = NULL;
// 	huffmanACTable = (struct HuffmanACTable *) malloc(sizeof(struct HuffmanACTable) * ACtableSize);	
// 	int i;
// 	char dataVector[dataSize] = {0};
// 	int temp;

// 	FILE *fp = NULL;
// 	FILE *encodedData = NULL;
// 	if(!(fp = fopen("huffmanDCTable","r")))
// 	{
// 		printf("huffmanDCTable not read \n");
// 		return -1;
// 	}
// 	for (i = 0; i < DCtableSize; ++i)
// 	{
// 		//fscanf(fp,"%d %s",&huffmanDCTable[i].size,huffmanDCTable[i].bitpattern);
// 		fscanf(fp,"%d %d",&huffmanDCTable[i].size,&huffmanDCTable[i].numberOfBits);
// 		huffmanDCTable[i].bitpattern = (char *) malloc(sizeof(char) * huffmanDCTable[i].numberOfBits);
// 		fscanf(fp,"%s",huffmanDCTable[i].bitpattern);
// 	}
// 	if(!(fp = fopen("huffmanACTable","r")))
// 	{
// 		printf("huffmanACTable not read \n");
// 		return -1;
// 	}
// 	for (i = 0; i < ACtableSize; ++i)
// 	{
// 		//fscanf(fp,"%d %s",&huffmanDCTable[i].size,huffmanDCTable[i].bitpattern);
// 		fscanf(fp,"%d %d %d",&huffmanACTable[i].run,&huffmanACTable[i].size,&huffmanACTable[i].numberOfBits);
// 		huffmanACTable[i].bitpattern = (char *) malloc(sizeof(char) * huffmanACTable[i].numberOfBits);
// 		fscanf(fp,"%s",huffmanACTable[i].bitpattern);
// 	}
// 	fclose(fp);
// 	// for (i = 0; i < ACtableSize; ++i)
// 	// {
// 	// 	//fscanf(fp,"%d %s",&huffmanDCTable[i].size,huffmanDCTable[i].bitpattern);
// 	// 	printf("%d %d %d %s\n",huffmanACTable[i].run,huffmanACTable[i].size,huffmanACTable[i].numberOfBits,huffmanACTable[i].bitpattern);
// 	// }	
	
// 	if(!(fp = fopen("zigzag_DCT_U","r")))
// 	{
// 		printf("zigzag_DCT_U \n");
// 		return -1;
// 	}
// 	if(!(encodedData = fopen("encodedData","w")))
// 	{
// 		printf("Error Opening encodedData \n");
// 		return -1;
// 	}
// 	i = 0;
// 	while((fscanf(fp, "%d ", &temp)) > 0) 
// 	{       

// 		dataVector[i] = temp;
// 		//printf("%d ",dataVector[i]);
// 		i++;
// 		if(i == dataSize - 1)
// 		{
// 			i = 0;
// 			entropyCoding(dataVector,huffmanDCTable,huffmanACTable,encodedData);
// 			//printf("\n");
// 		}
// 	    //printf("%d ",dataVector[i]); // Just to show output.
// 	    //i++;
// 	}
// 	fclose(encodedData);
// 	bitPacking("encodedData","U_byte_data");


// 	// for (i = 0; i < dataSize; ++i)
// 	// {
// 	// 	size = bitCount(data[i]);
// 	// 	printf("size:%d\n",size);
// 	// 	code = (int *) malloc(size*sizeof(int));
// 	// 	getBinary(data[i],code,size);
// 	// 	printf("\nNumber:%d Size:%d Binary:",data[i],size);
// 	// 	//binaryDisplay(code,size);
// 	// 	printf("\n");
// 	// 	for (j = 0; j < tableSize; ++j)
// 	// 	{
// 	// 		if(huffmanDCTable[j].size == size)
// 	// 		{
// 	// 			bitpattern = (char *) malloc(huffmanDCTable[j].numberOfBits * sizeof(char));
// 	// 			bitpattern = huffmanDCTable[j].bitpattern;
// 	// 			writeTofile(fp,bitpattern,huffmanDCTable[j].numberOfBits,code,size);
// 	// 			//free(bitpattern);
// 	// 			break;
// 	// 		}
// 	// 	}
		
// 	// 	free(code);
// 	// }
// 	fclose(fp);
// 	return 0;

// }