#include <stdio.h>
#include <stdlib.h>
#include "bmpReader.h"


BMPData* readBMPfile(const char *filename)
{
	BMPData *bmpData;
	bmpData = (BMPData*) malloc(sizeof(BMPData));
	FILE *fp;
	int ERROR;
	printf("%s\n",filename);
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
	printf(" size: %d \n width: %d \n height: %d \n planes: %d \n bits: %d \n",bmpData->infoHeader.size,bmpData->infoHeader.width,bmpData->infoHeader.height,bmpData->infoHeader.planes,bmpData->infoHeader.bits); 
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

int writeBMPfile(BMPData data,char *filename)
{

	int ERROR;
	FILE *image;
	if((image = fopen(filename,"wb")) == NULL)
	{
		printf("Error Reading File\n");
		fclose(image);
		return -1;
	}

	if((ERROR = fwrite(&data.header,sizeof(data.header),1,image)) != 1)
	{
		printf("Error Writing Header\n");
		printf("ERROR: %d \n",ERROR);
		fclose(image);
		return -1;
	}

	if(fwrite(&data.infoHeader,sizeof(data.infoHeader),1,image) != 1)
	{
		printf("Error Writing InfoHeader\n");
		fclose(image);
		return -1;
	}

	fseek(image,data.header.offset,SEEK_SET);
	if((ERROR = fwrite(&data.bitMapImage,sizeof(data.infoHeader.imagesize),1,image))  != 1) // check if sizeof is needed ?
	{
		printf("Error Writing Image BitMap\n");
		printf("ERROR:%d\n",ERROR);
		fclose(image);
		return -1;
	}
	free(data.bitMapImage);
	return 1;
}


