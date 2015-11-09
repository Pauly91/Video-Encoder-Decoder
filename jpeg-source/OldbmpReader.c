#include <stdio.h>
#include <stdlib.h>
#include <math.h>	
#include <string.h>

#include "bmpReader.h"

int returnScanf;

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





void downSample(BMPData *image, unsigned char ** Y, unsigned char ** U, unsigned char ** V, unsigned char **downSampledU, unsigned char ** downSampledV, int dSampleU, int dSampleV)
{

	int i,j,k,l;
	if(dSampleU != 4 && dSampleV  != 4)
	{

		if(dSampleU == dSampleV)
		{
			if(dSampleV == 1)
				dSampleU = 4;
			if(dSampleV == 2)
				dSampleV = 1;

			for (i = 0, l =0; i < image->infoHeader.height; i += dSampleV, l++) // check loop
			{
				for (j = 0,k = 0; j < image->infoHeader.width; j += dSampleU, k++) // check loop
				{
					 	downSampledU[l][k] = U[i][j];
					 	downSampledV[l][k] = V[i][j];

				}
			}
		}
		else
		{
			if(dSampleU == 4)
			{
				dSampleU = 1;
				dSampleV = 2;
			}
			else if(dSampleU == 2)
			{
				dSampleV = 2;
			}
			for (i = 0, l = 0; i < image->infoHeader.height; i += dSampleV, ++l) // check loop
			{
				for (j = 0, k = 0; j < image->infoHeader.width; j += dSampleU, ++k) // check loop
				{
				 	downSampledU[l][k] = U[i][j];
				 	downSampledV[l][k] = V[i][j];
				}
			}		
		}

	}
}


void quantize(BMPData *image, float **dct_Y, float **dct_U, float **dct_V, unsigned char ** luminanceQuantizationMatrix, unsigned char ** chrominanceQuantizationMatrix, int blockSize, int dSampleHeight, int dSampleWidth)
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
					//printf("%f/%u = ",dct_Y[u + i][v + j],luminanceQuantizationMatrix[u][v]);
					dct_Y[u + i][v + j] /= luminanceQuantizationMatrix[u][v];
					//printf("%f\n",dct_Y[u + i][v + j]);
				}
			}

		}
	}

	for (i = 0; i < dSampleHeight; i += blockSize)
	{
		for (j = 0; j < dSampleWidth; j += blockSize)
		{
			for (u = 0; u < blockSize; ++u)
			{
				for (v = 0; v < blockSize; ++v)
				{
					//printf("%f/%u = ",dct_U[u + i][v + j],chrominanceQuantizationMatrix[u][v]);
					dct_U[u + i][v + j] /= chrominanceQuantizationMatrix[u][v];
					dct_V[u + i][v + j] /= chrominanceQuantizationMatrix[u][v];
					//printf("%f %f ",dct_U[u + i][v + j],dct_V[u + i][v + j]);
				}
			}

		//printf("\n");
		}

	}

}

void zigzag(BMPData *image, float **dct,int blockSize, char *filename,int dSampleHeight, int dSampleWidth)
{
	int i,j,m,n;
	int height,width;
	int holder;
	FILE *fp;
	printf("%s\n",filename);
	if((fp = fopen(filename,"w")) == NULL)
	{
		printf("%s not open\n",filename);
		return ;
	}
	if(dSampleHeight != 0 )
	{
		height = dSampleHeight;
		width = dSampleWidth;
	}
	else
	{
		height = image->infoHeader.height;
		width = image->infoHeader.width;

	}
	//printf("height: %d width:%d\n",height,width);

	for (m = 0; m < height; m += blockSize)
	{
		for (n = 0; n < width; n += blockSize)
		{
			i = 0;
			j = 0;
			while( i != blockSize && j != blockSize)
			{

				if(j%2 == 0 && i == 0)
				{
					fprintf(fp,"%d ",(int) dct[m + i][n + j]);
					j++;
				}
				else if(j%2 == 1 && i == 0)
				{
					while(j != 0)
					{
						fprintf(fp,"%d ",(int) dct[m + i][n + j]);
						j -= 1;
						i += 1;
					}
				}
				else if(i%2 == 0 && j == 0)
				{
					while(i != 0)
					{
						fprintf(fp,"%d ",(int) dct[m + i][n + j]);
						j += 1;
						i -= 1;
					}			
				}
				else if(i%2 == 1 && j == 0)
				{
					if(i == blockSize - 1)
					{
						
						fprintf(fp,"%d ",(int) dct[m + i][n + j]);
						j++;
					}

					else
					{
						fprintf(fp,"%d ",(int) dct[m + i][n + j]);
						i++;
					}

				}
				else if(i == blockSize - 1 && j%2 == 1)
				{
					holder = j;
					while(i != holder)
					{
						fprintf(fp,"%d ",(int) dct[m + i][n + j]);
						j += 1;
						i -= 1;				
					}
				}
				else if(i == blockSize - 1 && j%2 == 0)
				{
						fprintf(fp,"%d ",(int) dct[m + i][n + j]);
						j++;
				}		
				else if(j == blockSize - 1 && i%2 == 1)
				{
					fprintf(fp,"%d ",(int) dct[m + i][n + j]);
					i++;
				}
				else if(j == blockSize - 1 && i%2 == 0)
				{
					holder = i;
					while(j != holder)
					{
						fprintf(fp,"%d ",(int) dct[m + i][n + j]);
						j -= 1;
						i += 1;				
					}
				}
				if(i == blockSize -1 && j == blockSize - 1)
				{
						fprintf(fp,"%d ",(int) dct[m + i][n + j]);
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
	if(value == 0)
	{
		return 0;
	}
	else
	{
		while(value)
		{
			value >>= 1;
			count++;
		}
		return count;	
	}

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

	number = abs(number);
	size--;
	if(number == 0)
	{
		code[0] = 0;
		return;
	}
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
}

void writeTofile(FILE *fp,char *bitpattern,int sizeOfBitPattern, int *code, int sizeOfCode)
{
	int i;
	for (i = 0; i < sizeOfBitPattern; ++i)
	{
		fprintf(fp,"%c",bitpattern[i]);
	}
	for (i = 0; i < sizeOfCode; ++i)
	{
		fprintf(fp,"%d",code[i]);
	}
}



void dpcm(char DC, struct HuffmanDCTable *huffmanDCTable, FILE *encodedData)
{
	int *code;
	int size;
	int j;
	int temp = DC;

	//printf("before -> DC: %d\n",DC);
	DC -= previousDC;
	//printf("DC: %d\n",DC);
	size = bitCount(DC);
	//printf("size:%d\n",size);
	if(size)
	{
		code = (int *) malloc(size*sizeof(int));
		getBinary(DC,code,size);
		//printf("Code:");
		binaryDisplay(code,size);
		//printf("\n");
	}
	else
	{
		size = 0;
		code = (int *) malloc(sizeof(int));
		code = 0;
		// size = 2;
		// code = (int *) malloc(size*sizeof(int));
		// code[0] = 0;
		// code[1] = 0;
		// printf("Code:");
		// binaryDisplay(code,size);
		// printf("\n");
	}
	for (j = 0; j < DCtableSize; ++j)
	{
		if(huffmanDCTable[j].size == size)
		{
			writeTofile(encodedData,huffmanDCTable[j].bitpattern,huffmanDCTable[j].numberOfBits,code,size);
			break;
		}
	}
	previousDC = temp;

}



// void dpcm(char DC, struct HuffmanDCTable *huffmanDCTable, FILE *encodedData)
// {
// 	int *code;
// 	int size;
// 	int j;
// 	//char *bitpattern;
// 	if(firstFlag == 1)
// 	{
// 		previousDC = DC;
// 		firstFlag = 0;
// 	}
// 	else
// 		DC -= previousDC;
// 	size = bitCount(DC);
// 	// printf("size:%d\n",size);
// 	code = (int *) malloc(size*sizeof(int));
// 	getBinary(DC,code,size);
// 	printf("\nNumber:%d Size:%d Binary:",DC,size);
// 	binaryDisplay(code,size);
// 	printf("\n");
// 	for (j = 0; j < DCtableSize; ++j)
// 	{
// 		if(huffmanDCTable[j].size == size)
// 		{
// 			// bitpattern = (char *) malloc(huffmanDCTable[j].numberOfBits * sizeof(char));
// 			// bitpattern = huffmanDCTable[j].bitpattern;
// 			writeTofile(encodedData,huffmanDCTable[j].bitpattern,huffmanDCTable[j].numberOfBits,code,size);
// 			break;
// 		}
// 	}

// }
void rlcEncode(int count,int data,struct HuffmanACTable *huffmanACTable,FILE *encodedData)
{
	int i;
	int size = bitCount(data);
	int *code;

	code = (int *) malloc(size * sizeof(int));

	getBinary(data,code,size);	
	for (i = 0; i < ACtableSize; ++i)
	{
		if(huffmanACTable[i].size == size && huffmanACTable[i].run == count)
		{
			writeTofile(encodedData,huffmanACTable[i].bitpattern,huffmanACTable[i].numberOfBits,code,size);	
			break;
		}
	}	
}


void rlc(char *dataVector,struct HuffmanACTable *huffmanACTable,FILE *encodedData, int dataSize)
{

	int i,count = 0;

	FILE *fp = NULL;

	if(!(fp = fopen("rle","a+")))
	{
		printf("rle not opened\n");
		return;
	}


	for (i = 1; i < dataSize; ++i) // dataSize = blocksize * blockSize
	{
		if(dataVector[i] == 0)
		{
			while(dataVector[i] == 0)
			{
				i++;
				if(i > dataSize - 1)
					break;
				count++;
			}
			if(i > dataSize - 1)
			{
				fprintf(fp,"%d %d %d <--------- \n",count,0,i);
				fprintf(encodedData,"1010");
			}
			else
			{
				fprintf(fp,"%d %d %d\n",count,dataVector[i],i);
				rlcEncode(count,dataVector[i],huffmanACTable,encodedData);
			}
			if(i == dataSize - 1)
			{
				fprintf(encodedData,"1010");
			}
			count = 0;
		}

		else
		{
			fprintf(fp,"%d %d %d\n",count,dataVector[i],i);
			rlcEncode(count,dataVector[i],huffmanACTable,encodedData);
		}
	}
	fclose(fp);
}

void entropyCoding(char *dataVector,struct HuffmanDCTable *huffmanDCTable,struct HuffmanACTable *huffmanACTable,FILE *encodedData, int dataSize)
{

	// printf("%d\n",dataVector[0]);
	dpcm(dataVector[0],huffmanDCTable,encodedData);
	rlc(dataVector,huffmanACTable,encodedData, dataSize);
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
		if(value == '1')
		{
			byte |= runTimeMask;
		}
		runTimeMask >>= 1;

		if(runTimeMask == 0)
		{
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


void differentialHuffmanRle(char *zigZagData, char * targetFile,char *byteData, int dataSize)
{
	
	struct HuffmanDCTable *huffmanDCTable = NULL;
	huffmanDCTable = (struct HuffmanDCTable *) malloc(sizeof(struct HuffmanDCTable) * DCtableSize);
	
	struct HuffmanACTable *huffmanACTable = NULL;
	huffmanACTable = (struct HuffmanACTable *) malloc(sizeof(struct HuffmanACTable) * ACtableSize);	

	char dataVector[dataSize];
	int temp,i;

	FILE *fp = NULL;
	FILE *encodedData = NULL;

	if(!(fp = fopen("huffmanDCTable","r")))
	{
		printf("huffmanDCTable not read \n");
		return ;
	}

	for (i = 0; i < DCtableSize; ++i)
	{
		returnScanf = fscanf(fp,"%d %d",&huffmanDCTable[i].size,&huffmanDCTable[i].numberOfBits);
		huffmanDCTable[i].bitpattern = (char *) malloc(sizeof(char) * huffmanDCTable[i].numberOfBits);
		returnScanf = fscanf(fp,"%s",huffmanDCTable[i].bitpattern);
	}
	fclose(fp);

	if(!(fp = fopen("newHuffmanACtable","r")))
	{
		printf("huffmanACTable not read \n");
		return ;
	}

	for (i = 0; i < ACtableSize; ++i)
	{
		returnScanf = fscanf(fp,"%d %d %d",&huffmanACTable[i].run,&huffmanACTable[i].size,&huffmanACTable[i].numberOfBits);
		huffmanACTable[i].bitpattern = (char *) malloc(sizeof(char) * huffmanACTable[i].numberOfBits);
		returnScanf = fscanf(fp,"%s",huffmanACTable[i].bitpattern);
	}
	fclose(fp);
	
	if(!(fp = fopen(zigZagData,"r")))
	{
		printf("%s not opened \n",zigZagData);
		return ;
	}
	if(!(encodedData = fopen(targetFile,"w")))
	{
		printf("Error Opening %s \n",targetFile);
		return ;
	}
	i = 0;
	previousDC = 0;
	while((returnScanf = fscanf(fp, "%d ", &temp)) > 0) 
	{       

		dataVector[i] = temp;
		i++;
		if(i == dataSize)
		{
			// printf("i:%d\n",i);
			// i = 0;
			// for (i = 0; i < dataSize; ++i)
			// {
			// 	printf("%d ",dataVector[i]);
			// }
			i = 0;
			// printf("\n");
			entropyCoding(dataVector,huffmanDCTable,huffmanACTable,encodedData,dataSize);
		}
	}
	fclose(encodedData);
	bitPacking(targetFile,byteData);
	fclose(fp);
}

struct HuffmanDCTableDecode
{
	int size;
	int codeLength;
	char * code;
};

struct nodes
{
	char bit;
	int size;
	int codeLength;
	int run;
	char *code;
	struct nodes *left,*right;
};

struct HuffmanACTableDecode
{
	int run;
	int size;
	int codeLength;
	char *code;
}; 

struct nodes *DCHead = NULL;
struct nodes *ACHead = NULL;

void createNode(struct nodes *node)
{
	node = (struct nodes *) malloc(sizeof(struct nodes));
}

void printPreOrder(struct nodes *tree)
{
	if(tree)
	{
		printf("bit: %c size: %d codeLength: %d code: %s \n",tree->bit,tree->size,tree->codeLength,tree->code);
		printPreOrder(tree->left);
		printPreOrder(tree->right);
	}
}
// Over creating things
void treeCreatorDC(struct nodes *node, struct HuffmanDCTableDecode *huffmanDCTable, int index,int count)
{

	// //printf("%d %d %d %s\n",count,huffmanDCTable[count].size, huffmanDCTable[count].codeLength, huffmanDCTable[count].code);
	// //printf("index: %d bit: %c \n",index,huffmanDCTable[count].code[index]);
	if(huffmanDCTable[count].code[index] == '0')
	{
		//printf("Left->bit: %c \n",huffmanDCTable[count].code[index]);	
		if(node->left == NULL)
		{
			node->left = (struct nodes *) malloc(sizeof(struct nodes));
			node = node->left;
			node->left = NULL;
			node->right = NULL;
			node->bit = huffmanDCTable[count].code[index];
			//printf("-->new node\n");
		}
		else
			node = node->left;
		//printf("**Left**\nbit: %c bit %c\n",node->bit,huffmanDCTable[count].code[index]);

	}
	else
	{
		//printf("Right->bit: %c \n",huffmanDCTable[count].code[index]);
		if(node->right == NULL)
		{
			node->right = (struct nodes *) malloc(sizeof(struct nodes));
			node = node->right;
			node->left = NULL;
			node->right = NULL;
			node->bit = huffmanDCTable[count].code[index];
			//printf("-->new node\n");
		}
		else
			node = node->right;
		//printf("**Right**\nbit: %c bit %c\n",node->bit,huffmanDCTable[count].code[index]);
	}
	node->size = -1;
	node->codeLength = -1;
	node->code = NULL;
	//printf("index: %d codeLength: %d \n\n\n",index,huffmanDCTable[count].codeLength - 1);
	if(index == huffmanDCTable[count].codeLength - 1)
	{
		node->size = huffmanDCTable[count].size;
		node->codeLength = huffmanDCTable[count].codeLength;
		node->code = (char *) malloc(node->codeLength * sizeof(char));
		strcpy(node->code,huffmanDCTable[count].code);
		node->left = NULL;
		node->right = NULL;
		//printf("\n*****Hit Leaf Node*******\n\n\n");
		if(count == DCtableSize -1)
			return;
		//printf("DCHead->bit:%c\n",DCHead->bit);
		treeCreatorDC(DCHead,huffmanDCTable,0,++count);
		
	}
	else
	{
		treeCreatorDC(node,huffmanDCTable,++index,count);
	}
	return;

}

void treeCreatorAC(struct nodes *node, struct HuffmanACTableDecode *huffmanACTable, int index,int count)
{

	//printf("%d %d %d %s\n",count,huffmanACTable[count].size, huffmanACTable[count].codeLength, huffmanACTable[count].code);
	//printf("index: %d bit: %c \n",index,huffmanACTable[count].code[index]);
	if(huffmanACTable[count].code[index] == '0')
	{
		//printf("Left->bit: %c \n",huffmanACTable[count].code[index]);	
		if(node->left == NULL)
		{
			node->left = (struct nodes *) malloc(sizeof(struct nodes));
			node = node->left;
			node->left = NULL;
			node->right = NULL;
			node->bit = huffmanACTable[count].code[index];
			//printf("-->new node\n");
		}
		else
			node = node->left;
		//printf("**Left**\nbit: %c bit %c\n",node->bit,huffmanACTable[count].code[index]);

	}
	else
	{
		//printf("Right->bit: %c \n",huffmanACTable[count].code[index]);
		if(node->right == NULL)
		{
			node->right = (struct nodes *) malloc(sizeof(struct nodes));
			node = node->right;
			node->left = NULL;
			node->right = NULL;
			node->bit = huffmanACTable[count].code[index];
			//printf("-->new node\n");
		}
		else
			node = node->right;
		//printf("**Right**\nbit: %c bit %c\n",node->bit,huffmanACTable[count].code[index]);
	}
	node->size = -1;
	node->codeLength = -1;
	node->code = NULL;
	//printf("index: %d codeLength: %d \n\n\n",index,huffmanACTable[count].codeLength - 1);
	if(index == huffmanACTable[count].codeLength - 1)
	{
		node->size = huffmanACTable[count].size;
		node->codeLength = huffmanACTable[count].codeLength;
		node->run = huffmanACTable[count].run;
		node->code = (char *) malloc(node->codeLength * sizeof(char));
		strcpy(node->code,huffmanACTable[count].code);
		node->left = NULL;
		node->right = NULL;
		//printf("\n*****Hit Leaf Node*******\n\n\n");
		if(count == ACtableSize -1)
			return;
		//printf("ACHead->bit:%c\n",ACHead->bit);
		treeCreatorAC(ACHead,huffmanACTable,0,++count);
		
	}
	else
	{
		treeCreatorAC(node,huffmanACTable,++index,count);
	}
	return;

}


int numberFinder(char * code,int size)
{
	int i;
	int mask = 1;
	int number = 0;
	//printf("size: %d\n",size);
	if(code[0] == '0')
	{
		for (i = size-1; i >= 0; --i)
		{
			//printf("code[%d]: %c\n",i,code[i]);
			if(code[i] == '0')
			{
				number |= mask;
			}
			mask <<= 1;
		}
			return -1*number;
	}
	else
	{
		for (i = size-1; i >= 0; --i)
		{
			//printf("code[%d]: %c\n",i,code[i]);
			if(code[i] == '1')
			{
				number |= mask;
			}
			mask <<= 1;
		}
		return number;	
	}




}

struct nodes * findCode(struct nodes *node, char bit)
{
	if(bit == '0')
		return node->left;
	else
		return node->right;
}



int compare_function(const void *a,const void *b) 
{
	struct HuffmanACTableDecode *x = (struct HuffmanACTableDecode *) a;
	struct HuffmanACTableDecode *y = (struct HuffmanACTableDecode *) b;
	return strcmp(x->code,y->code);
	// if (x->probabilty < y->probabilty) 
	// 	return 1;
	// else if (x->probabilty > y->probabilty) 
	// 	return -1; 
	// return 0;
}


void byteToBitConverter(char * fileName,char *target, int MaxPixel)
{
	char byte;
	int i;
	int count = 0;
	unsigned char mask = 0b10000000;
	unsigned char runMask;
	FILE *fp = NULL, *bitFile = NULL;
	
	if (!(fp = fopen(fileName,"r")))
	{
		printf("%s not open\n",fileName);
		return;
	}
	if (!(bitFile = fopen(target,"w")))
	{
		printf("%s not open\n",target);
		return;
	}
	while(returnScanf = fscanf(fp,"%c",&byte) > 0)
	{
		runMask = mask;
		for (i = 0; i < byteSize; ++i)
		{
			
			if(byte & runMask)
			{
				fprintf(bitFile,"1 ");
			}
			else
			{
				fprintf(bitFile,"0 ");
			}
			runMask >>= 1;
			count++;
			if(count > MaxPixel -1)
			{
				count = 0;
				break;
			}
		}
	}
	fprintf(bitFile,"\n");
	fclose(fp);
	fclose(bitFile);
}

void decodeRleHuffman(char * byteFile, char *decodeZigZag, int numberOfBlockHeight, int numberOfBlockWidth, int MaxPixel)
{
	FILE *fp = NULL,*decode = NULL;
	
	char bit;
	char *patternHolder = NULL;

	int i,j;
	int endOfFrameCount = 0;
	int dcFlag = 0;
	int DCValue,ACValue;
	int size;
	int endOfBlock = 0;
	int numberOfPixel = 0;
	int numberOfZero;
	int value = 0;

	struct HuffmanDCTableDecode *huffmanDCTable;
	struct HuffmanACTableDecode *huffmanACTable = NULL;
	struct nodes *holder = NULL;
	//holder = (struct nodes *) malloc(sizeof(struct nodes));
	huffmanACTable = (struct HuffmanACTableDecode *) malloc(sizeof(struct HuffmanACTableDecode) * ACtableSize);	
	DCHead = (struct nodes *) malloc(sizeof(struct nodes));
	DCHead->left = NULL;
	DCHead->right = NULL;
	DCHead->bit = 'H';

	ACHead = (struct nodes *) malloc(sizeof(struct nodes));
	ACHead->left = NULL;
	ACHead->right = NULL;
	ACHead->bit = 'H';
	byteToBitConverter(byteFile,"data_unpacked", MaxPixel);
	huffmanDCTable = (struct HuffmanDCTableDecode *) malloc(DCtableSize * sizeof(struct HuffmanDCTableDecode));
	if (!(fp = fopen("huffmanDCTable","r")))
	{
		printf("huffmanDCTable not opened\n");
		return;
	}	
	for (i = 0; i < DCtableSize; ++i)
	{
		returnScanf = fscanf(fp,"%d %d",&huffmanDCTable[i].size,&huffmanDCTable[i].codeLength);
		huffmanDCTable[i].code = (char *) malloc(huffmanDCTable[i].codeLength * sizeof(char));
		returnScanf = fscanf(fp,"%s",huffmanDCTable[i].code);
		////printf("%d %d %s\n",huffmanDCTable[i].size, huffmanDCTable[i].codeLength, huffmanDCTable[i].code);
	}
	fclose(fp);
	treeCreatorDC(DCHead,huffmanDCTable,0,0);
	//printf("\n**PreOrder**\n");
	//printPreOrder(DCHead);	
	if(!(fp = fopen("newHuffmanACtable","r")))
	{
		printf("huffmanACTable not read \n");
		return;
	}
	for (i = 0; i < ACtableSize; ++i)
	{
		//returnScanf = fscanf(fp,"%d %s",&huffmanDCTable[i].size,huffmanDCTable[i].code);
		returnScanf = fscanf(fp,"%d %d %d",&huffmanACTable[i].run,&huffmanACTable[i].size,&huffmanACTable[i].codeLength);
		huffmanACTable[i].code = (char *) malloc(sizeof(char) * huffmanACTable[i].codeLength);
		returnScanf = fscanf(fp,"%s",huffmanACTable[i].code);
	}
	fclose(fp);

	qsort(huffmanACTable,ACtableSize,sizeof(struct HuffmanACTableDecode),compare_function);	

	treeCreatorAC(ACHead,huffmanACTable,0,0);
	//printf("\n**PreOrder**\n");
	//printPreOrder(ACHead);	

	if(!(fp = fopen("data_unpacked","r")))
	{
		printf("data_unpacked not read \n");
		return;
	}
	if(!(decode = fopen(decodeZigZag,"w")))
	{
		printf("decode not read \n");
		return;
	}
	
	previousDC = 0;
	while(endOfFrameCount < numberOfBlockHeight * numberOfBlockWidth) // should be 64
	{

		holder = DCHead;
		numberOfPixel = 0;		
		while(!dcFlag) // reset all init values for the loop
		{
			//printf("\n*** New Bit DC -- %s***\n",decodeZigZag);
			returnScanf = fscanf(fp,"%c ",&bit);
			//printf("bit Read:%c \n",bit);
			holder = findCode(holder,bit);
			//printf("value: %c\n",holder->bit); 
			//printf("code: %s\n",holder->code);
			if(holder->size >= 0)
			{
				size = holder->size;
				//printf("size: %d\n",size);
				if(size == 0)
				{
					DCValue = 0;
				}
				else
				{
					patternHolder = (char *) malloc(size * sizeof(char));
					i = 0;
					//printf("Data bits:");
					while(i < size )
					{

						returnScanf = fscanf(fp,"%c ",&patternHolder[i]); // 0 has the msb
						//printf("%c ",patternHolder[i]);
						i++;
					}
					//printf("\n");
					//printf("code :%s\n",patternHolder);
					DCValue = numberFinder(patternHolder,size); // use of previous DC value
					//printf("DC value Before DPCM: %d\n",DCValue);
				}	
				DCValue += previousDC;
				previousDC = DCValue;
				//printf("number: %d\n",DCValue);
				numberOfPixel++;
				fprintf(decode, "%d ",DCValue);
				holder = ACHead;
				while(!endOfBlock)
				{
					
					//printf("\n*** New Bit AC %s***\n",decodeZigZag);
					//printf("Frame: %d pixel:%d\n",endOfFrameCount,numberOfPixel);
					returnScanf = fscanf(fp,"%c ",&bit);
					//printf("bit Read:%c \n",bit);
					holder = findCode(holder,bit);
					//printf("%c",holder->bit);
					if(holder->size >= 0)
					{
						size = holder->size;
						if(size == 0)
						{
							// put the zeros in files correspoding to numberOfPixel Count
							for (j = numberOfPixel; j < MaxPixel- 1; ++j)
							{
								fprintf(decode, "%d ",value);	
							}
							//printf("\nnumber: %d\n",0);
							fprintf(decode,"%d\n",value);
							endOfFrameCount++;	
							break;
						}
						numberOfZero = holder->run;
						//printf("\nsize: %d\n",size);
						patternHolder = (char *) malloc(size * sizeof(char));
						i = 0;
						while(i < size )
						{
							
							returnScanf = fscanf(fp,"%c ",&patternHolder[i]); // 0 has the msb
						//	printf("%c ",patternHolder[i]);
							i++;
						}
						//printf("\n");

						//printf("code :%s\n",patternHolder);
						ACValue = numberFinder(patternHolder,size); // get the number of Zeros right
						//printf("number: %d\n",ACValue);
						for (j = 0; j < numberOfZero; ++j)
						{
							numberOfPixel++;
							fprintf(decode, "%d ",value);	
						}
						numberOfPixel++;
						fprintf(decode, "%d ",ACValue);	
						holder = ACHead;
					}
				}

				break;
			}
		}
	}
	fclose(fp);
	fclose(decode); 
}

void reAssembleZigZag(char * zigZagFile, int **reblock, int numberOfBlockHeight, int numberOfBlockWidth, int blockSize)
{

	int i = 0,j = 0, x = 0, y = 0;
	int holder;
	int value;

	FILE *fp;
	if(!(fp = fopen(zigZagFile,"r")))
	{
		printf("%s not opened \n",zigZagFile);
		return;
	}
	for (x = 0; x < numberOfBlockHeight; x += blockSize)
	{
		for (y = 0; y < numberOfBlockWidth; y += blockSize)
		{
			i = j = 0;
			while( i != blockSize && j != blockSize)
			{
				if(j%2 == 0 && i == 0)
				{
					//sscanf(line,"%d",&reblock[x + i][y + j]);
					returnScanf = fscanf(fp,"%d ",&value);
					if(returnScanf < 0)
						printf("Error-------> %c %d \n",value,value);
					reblock[x + i][y + j] = value;

					//printf("reblock[%d][%d]:%d i:%d j:%d return: %d value:%c %d\n",i + x,y + j,reblock[i + x][y + j],i,j,returnScanf,value,value);
					j++;
				}
				else if(j%2 == 1 && i == 0)
				{
					while(j != 0)
					{
						//sscanf(line,"%d",&reblock[x + i][y + j]);
						returnScanf = fscanf(fp,"%d ",&value);
						if(returnScanf < 0)
							printf("Error-------> %c %d \n",value,value);
						reblock[x + i][y + j] = value;

						//printf("reblock[%d][%d]:%d i:%d j:%d return: %d value:%c %d\n",i + x,y + j,reblock[i + x][y + j],i,j,returnScanf,value,value);
						j -= 1;
						i += 1;
					}
				}
				else if(i%2 == 0 && j == 0)
				{
					while(i != 0)
					{
						//sscanf(line,"%d",&reblock[x + i][y + j]);
						returnScanf = fscanf(fp,"%d ",&value);
						if(returnScanf < 0)
							printf("Error-------> %c %d \n",value,value);
						reblock[x + i][y + j] = value;

						//printf("reblock[%d][%d]:%d i:%d j:%d return: %d value:%c %d\n",i + x,y + j,reblock[i + x][y + j],i,j,returnScanf,value,value);
						j += 1;
						i -= 1;
					}			
				}
				else if(i%2 == 1 && j == 0)
				{
					if(i == blockSize - 1)
					{
						//sscanf(line,"%d",&reblock[x + i][y + j]);
						returnScanf = fscanf(fp,"%d ",&value);
						if(returnScanf < 0)
							printf("Error-------> %c %d \n",value,value);
						reblock[x + i][y + j] = value;

						//printf("reblock[%d][%d]:%d i:%d j:%d return: %d value:%c %d\n",i + x,y + j,reblock[i + x][y + j],i,j,returnScanf,value,value);
						j++;
					}

					else
					{
						//sscanf(line,"%d",&reblock[x + i][y + j]);
						returnScanf = fscanf(fp,"%d ",&value);
						if(returnScanf < 0)
							printf("Error-------> %c %d \n",value,value);
						reblock[x + i][y + j] = value;

						//printf("reblock[%d][%d]:%d i:%d j:%d return: %d value:%c %d\n",i + x,y + j,reblock[i + x][y + j],i,j,returnScanf,value,value);
						i++;
					}

				}
				else if(i == blockSize - 1 && j%2 == 1)
				{
					holder = j;
					while(i != holder)
					{
						//sscanf(line,"%d",&reblock[x + i][y + j]);
						returnScanf = fscanf(fp,"%d ",&value);
						if(returnScanf < 0)
							printf("Error-------> %c %d \n",value,value);
						reblock[x + i][y + j] = value;

						//printf("reblock[%d][%d]:%d i:%d j:%d return: %d value:%c %d\n",i + x,y + j,reblock[i + x][y + j],i,j,returnScanf,value,value);
						j += 1;
						i -= 1;				
					}
				}
				else if(i == blockSize - 1 && j%2 == 0)
				{
					//sscanf(line,"%d",&reblock[x + i][y + j]);
					returnScanf = fscanf(fp,"%d ",&value);
					if(returnScanf < 0)
						printf("Error-------> %c %d \n",value,value);
					reblock[x + i][y + j] = value;

					//printf("reblock[%d][%d]:%d i:%d j:%d return: %d value:%c %d\n",i + x,y + j,reblock[i + x][y + j],i,j,returnScanf,value,value);
					j++;
				}		
				else if(j == blockSize - 1 && i%2 == 1)
				{
					//sscanf(line,"%d",&reblock[x + i][y + j]);
					returnScanf = fscanf(fp,"%d ",&value);
					if(returnScanf < 0)
						printf("Error-------> %c %d \n",value,value);
					reblock[x + i][y + j] = value;

					//printf("reblock[%d][%d]:%d i:%d j:%d return: %d value:%c %d\n",i + x,y + j,reblock[i + x][y + j],i,j,returnScanf,value,value);
					i++;
				}
				else if(j == blockSize - 1 && i%2 == 0)
				{
					holder = i;
					while(j != holder)
					{
						//sscanf(line,"%d",&reblock[x + i][y + j]);
						returnScanf = fscanf(fp,"%d ",&value);
						if(returnScanf < 0)
							printf("Error-------> %c %d \n",value,value);
						reblock[x + i][y + j] = value;

						//printf("reblock[%d][%d]:%d i:%d j:%d return: %d value:%c %d\n",i + x,y + j,reblock[i + x][y + j],i,j,returnScanf,value,value);
						j -= 1;
						i += 1;				
					}
				}

				if((i == blockSize -1) && (j == blockSize - 1))
				{
					//sscanf(line,"%d",&reblock[x + i][y + j]);
					returnScanf = fscanf(fp,"%d ",&value);
					if(returnScanf < 0)
						printf("Error-------> %c %d \n",value,value);
					reblock[x + i][y + j] = value;

					//printf("reblock[%d][%d]:%d i:%d j:%d return: %d value:%c %d\n",i + x,y + j,reblock[i + x][y + j],i,j,returnScanf,value,value);
					i++;
					j++;
					break;
				}

			}
			// for (p = 0; p < 8; ++p)
			// {
			// 	for (q = 0; q < 8; ++q)
			// 	{
			// 		printf("%d ",reblock[p + x][y + q]);
			// 	}
			// }
			//printf("\n");
			fflush(fp); 
		}
	}
	// printf("\nx:%d y:%d\n",x,y);
	// printf("\n\n** end ** \n\n");	
	fclose(fp);
}


void deQuantize(BMPData *image, int **dct_Y, int **dct_U, int **dct_V, unsigned char ** luminanceQuantizationMatrix, unsigned char ** chrominanceQuantizationMatrix, int blockSize, int dSampleHeight, int dSampleWidth)
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
					//printf("dct_Y[%d][%d]:%d*%u = ",u + i,v + j,dct_Y[u + i][v + j],luminanceQuantizationMatrix[u][v]);
					dct_Y[u + i][v + j] *= luminanceQuantizationMatrix[u][v];
					//printf("%d\n",dct_Y[u + i][v + j]);
				}
			}
			//printf("\n");

		}
	}

	for (i = 0; i < dSampleHeight; i += blockSize)
	{
		for (j = 0; j < dSampleWidth; j += blockSize)
		{
			for (u = 0; u < blockSize; ++u)
			{
				for (v = 0; v < blockSize; ++v)
				{
					//printf("dct_V[%d][%d]:%d*%u = ",u + i,v + j,dct_V[u + i][v + j],chrominanceQuantizationMatrix[u][v]);
					dct_U[u + i][v + j] *= chrominanceQuantizationMatrix[u][v];
					dct_V[u + i][v + j] *= chrominanceQuantizationMatrix[u][v];
					//printf("%d \n",dct_V[u + i][v + j]);
				}
			}
			//printf("\n");
		}
	}

}



void DCT(BMPData *image, float **dct_Y, float **dct_U, float **dct_V, unsigned char ** Y, unsigned char ** U, unsigned char ** V, int blockSize, int dSampleHeight, int dSampleWidth)
{

	int i,j,u,v,x,y;
	float value = 0, value1 = 0, value2 = 0;
	float cu,cv;
	for (i = 0; i < image->infoHeader.height; i += blockSize)
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize)
		{
			for (u = 0; u < blockSize; ++u)
			{
				if (u == 0)
					 cu = 1/sqrt(2);
				else
					cu = 1;			
				for (v = 0; v < blockSize; ++v)
				{
					if(v == 0)
						cv = 1/sqrt(2);
					else
						cv = 1;						
					value = 0;	
					for (x = 0; x < blockSize; ++x)
					{
						for (y = 0; y < blockSize; ++y)
						{
							value += Y[x + i][y + j] * cos(pi *(2*x + 1)*u/16) * cos(pi *(2*y + 1)*v/16);
						}
					}
					dct_Y[u + i][v + j] = cu * cv * value * 0.25;// / luminanceQuantizationMatrix[u][v];
				}

			}

		}
	}

	for (i = 0; i < dSampleHeight; i += blockSize)
	{
		for (j = 0; j < dSampleWidth; j += blockSize)
		{
			for (u = 0; u < blockSize; ++u)
			{
				if (u == 0)
					 cu = 1/sqrt(2);
				else
					cu = 1;					
				for (v = 0; v < blockSize; ++v)
				{
					if(v == 0)
						cv = 1/sqrt(2);
					else
						cv = 1;					
					value1 = 0;
					value2 = 0;	
					for (x = 0; x < blockSize; ++x)
					{
						for (y = 0; y < blockSize; ++y)
						{
							value1 += U[x + i][y + j] * cos(pi *(2*x + 1)*u/16) * cos(pi *(2*y + 1)*v/16);
							value2 += V[x + i][y + j] * cos(pi *(2*x + 1)*u/16) * cos(pi *(2*y + 1)*v/16);
						}
					}

					dct_U[u + i][v + j] = cu * cv * value1 * 0.25;// / chrominanceQuantizationMatrix[u][v];
					dct_V[u + i][v + j] = cu * cv * value2 * 0.25;
				}
			}

		}
	}
}

void IDCT(BMPData *image, int **dct_Y, int **dct_U, int **dct_V, unsigned char ** Y, unsigned char ** U, unsigned char ** V, int blockSize, int dSampleHeight, int dSampleWidth)
{
	int i,j,u,v,x,y;
	float value = 0, value1 = 0, value2 = 0;
	float cu,cv;
	for (i = 0; i < image->infoHeader.height; i += blockSize)
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize)
		{
			for (x = 0; x < blockSize; ++x)
			{
				for (y = 0; y < blockSize; ++y)
				{
					value = 0;	
					for (u = 0; u < blockSize; ++u)
					{
						if (u == 0)
							 cu = 1/sqrt(2);
						else
							cu = 1;
						for (v = 0; v < blockSize; ++v)
						{
							if(v == 0)
								cv = 1/sqrt(2);
							else
								cv = 1;
							// if(i*j > 504 * 480)
							// 	printf("-- value:%f dct_Y[%d][%d]:%u \n",value,u + i,v + j,dct_Y[u + i][v + j]);
							value += cu * cv * dct_Y[u + i][v + j] * cos(pi * ((2*x + 1)*u) /16) * cos(pi *( (2*y + 1)*v)/16);
							//printf("value:%f\n",value);
						}
					}
					// printf("x:%d y:%d value:%f \n",x+i,y+j,round(0.25*value));
					Y[x + i][y + j] = round(0.25*value);// / luminanceQuantizationMatrix[u][v];	
					
				}

			}

		}
	}

	for (i = 0; i < dSampleHeight; i += blockSize)
	{
		for (j = 0; j < dSampleWidth; j += blockSize)
		{
			for (x = 0; x < blockSize; ++x)
			{
				for (y = 0; y < blockSize; ++y)
				{
					value1 = 0;
					value2 = 0;	
					for (u = 0; u < blockSize; ++u)
					{
						if (u == 0)
							cu = 1/sqrt(2);
						else
							cu = 1;						
						for (v = 0; v < blockSize; ++v)
						{
							if(v == 0)
								cv = 1/sqrt(2);
							else
								cv = 1;

							value1 += cu * cv * dct_U[u + i][v + j] * cos(pi *(2*x + 1)*u/16) * cos(pi *(2*y + 1)*v/16);
							value2 += cu * cv * dct_V[u + i][v + j] * cos(pi *(2*x + 1)*u/16) * cos(pi *(2*y + 1)*v/16);
						}
					}

					U[x + i][y + j] = round(0.25*value1);// / chrominanceQuantizationMatrix[u][v];
					V[x + i][y + j] = round(0.25*value2);
				}
			}

		}
	}
}

void upSample(BMPData *image, unsigned char ** Y, unsigned char ** U, unsigned char ** V, unsigned char **downSampledU, unsigned char ** downSampledV, int dSampleU, int dSampleV, int dSampleHeight, int dSampleWidth)
{

	int i,j,k,l,x,y;
	if(dSampleU != 4 && dSampleV  != 4)
	{

		if(dSampleU == dSampleV)
		{
			if(dSampleV == 1)
				dSampleU = 4;
			if(dSampleV == 2)
				dSampleV = 1;

			for (i = 0; i < dSampleHeight; ++i) // check loop
			{
				for (j = 0; j < dSampleWidth; ++j) // check loop
				{
					 for (l = 0; l < dSampleV; ++l)
					 {
					 	for (k = 0; k < dSampleU; ++k)
					 	{
						 	U[i + l][j + k] = downSampledU[i][j];
						 	V[i + l][j + k] = downSampledV[i][j];
					 	}
					 }
				}
			}
		}
		else
		{
			if(dSampleU == 4)
			{
				dSampleU = 1;
				dSampleV = 2;
			}
			else if(dSampleU == 2)
			{
				dSampleV = 2;
			}
			//printf("qwerty---> dSampleU:%d dSampleV:%d \n",dSampleU,dSampleV);
			for (i = 0, x = 0; i < image->infoHeader.height; i += dSampleV, x++) // check loop
			{
				for (j = 0, y = 0; j < image->infoHeader.width; j += dSampleU, y++) // check loop
				{
					 for (l = 0; l < dSampleV; ++l)
					 {
					 	for (k = 0; k < dSampleU; ++k)
					 	{
						 	U[i + l][j + k] = downSampledU[x][y];
						 	V[i + l][j + k] = downSampledV[x][y];
					 	}
					 }

				}
			}	
		}

	}
}