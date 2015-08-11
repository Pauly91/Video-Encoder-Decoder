/*
Author - Abel Paul Babu
Roll Number - MT2014501

Code for - 

	Reads the block data
	Assembles them back to BMP
	Using the following header data

**Header**
 type: 0x4d42 
 fileSize: 786570 
 reserved1: 0 
 offset: 138 


**InfoHeader**
 size: 124 
 width: 512 
 height: 512 
 planes: 1 
 bits: 24 
 compression: 0 
 imagesize: 786432 
 xResolution: 0 
 yResolution: 0 
 coloursUsed: 0 
 importantColours: 0 


reference -
	 http://paulbourke.net/dataformats/bmp/
	 http://www.ue.eti.pg.gda.pl/fpgalab/zadania.spartan3/zad_vga_struktura_pliku_bmp_en.html

*/


	

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>	 

//#define block 8

#pragma pack(push, 1)

 /* 
 	No space is wasted on padding - For correctness of data read as a structure when
 	using fread
 */


typedef struct Headers
{
	unsigned short int type;                 /* Magic identifier            */
	unsigned int fileSize;                       /* File size in bytes          */
	unsigned int reserved;
	unsigned int offset;                     /* Offset to image data, bytes */
} Header;


typedef struct InfoHeaders
{
	unsigned int size;               /* Header size in bytes      */
	int width,height;                /* Width and height of image */
	unsigned short int planes;       /* Number of colour planes   */
	unsigned short int bits;         /* Bits per pixel            */
	unsigned int compression;        /* Compression type          */
	unsigned int imagesize;          /* Image size in bytes       */
	int xResolution,yResolution;     /* Pixels per meter          */
	unsigned int coloursUsed;           /* Number of colours         */
	unsigned int importantColours;   /* Important colours         */
} InfoHeader;

#pragma pack(pop) // get back the orignial byte packing 


// void initHeaders(Header *header,InfoHeader *infoHeader)
// {
// 	header->type = 0x4d42 ;
// 	header->fileSize = 786570 ;
// 	header->reserved = 0;
// 	header->offset = 138;

// 	infoHeader->size = 124;
// 	infoHeader->width = 512;
// 	infoHeader->height = 512;
// 	infoHeader->
// 	infoHeader->
// 	infoHeader->
// 	infoHeader->
// 	infoHeader->
// 	infoHeader->
// }


int main(int argc, char const *argv[])
{
	// if(argc != 2)
	// {
	// 	printf("\n\nThe format is ./imageAssemble [file name]\n\n");
	// 	return -1;
	// }

	int i,j,k,l,ERROR;
	unsigned char *bitMapImage;
	int block,height,width,value;
	FILE *fp,*image;
	Header header;
	InfoHeader infoHeader;

	//initHeaders(&header,&infoHeader);



	
	if((fp = fopen("blockFile","r")) == NULL)
	{
		printf("Error Reading File\n");
		fclose(fp);
		return -1;
	}

	fscanf(fp,"%d %d %d ",&block,&height,&width);
	printf("%d %d %d\n",block,height,width);

	bitMapImage = (unsigned char *) malloc(height * width * 3 * sizeof(char));
	//printf("%d\n",height * width * 3 * sizeof(char));
	for (i = 0,l = 0; i < height/block; i++)
	{
		for (j = 0; j < width ; j++) // check if its block * block
		{
			
			for (k = 0,l = 0; k < block  ; ++k, l += 3)
			{	
				fscanf(fp,"%d ",&value);
				bitMapImage[k + width * j  + i * block + l] = (unsigned char) value;
				//bitMapImage[k + width * j  + i * block + l] = (unsigned char)value;
				//printf("%d\n",k + width * j  + i * block + l);
			//	printf("%d ",bitMapImage[k + width * j  + i * block + l ]);
			}	
			//printf("\n");	
		}
	}
	//printf("green\n");	
	//blue[k + width * j  + i * block]
	for (i = 0,l = 0; i < height/block; i++)
	{
		for (j = 0; j < width ; j++) // check if its block * block
		{
			
			for (k = 0,l = 0; k < block  ; ++k, l += 3)
			{	
				fscanf(fp,"%d ",&value);
				bitMapImage[k + width * j  + i * block + l + 1] = (unsigned char)value;
			//	printf("%d ",bitMapImage[k + width * j  + i * block + l + 1]);
			}	
			//printf("\n");	
		}

	}
	//printf("blue\n");	
	for (i = 0,l = 0; i < height/block; i++)
	{
		for (j = 0; j < width ; j++) // check if its block * block
		{
			
			for (k = 0,l = 0; k < block  ; ++k, l += 3)
			{	
				fscanf(fp,"%d ",&value);
				bitMapImage[k + width * j  + i * block + l + 2] = (unsigned char)value;
				//printf("%d\n",k + width * j  + i * block);
			//	printf("%d ",bitMapImage[k + width * j  + i * block + l + 2]);
			}	
			//printf("\n");	
		}

	}
	if((ERROR = fread(&header,sizeof(header),1,fp)) != 1)
	{
		printf("Error Reading Header\n");
		printf("ERROR: %d \n",ERROR);
		fclose(fp);
		return -1;
	}

	if (header.type != 0x4d42)
	{
		printf("Not a BMP file\n");
		fclose(fp);
		return -1;
	}
	// printf("**Header**\n");
	// printf(" type: 0x%x \n fileSize: %d \n reserved1: %d \n offset: %d \n",header.type,header.fileSize,header.reserved,header.offset);

	if((ERROR = fread(&infoHeader,sizeof(infoHeader),1,fp)) != 1)
	{
		printf("Error Reading InfoHeader\n");
		printf("ERROR: %d \n",ERROR);
		fclose(fp);
		return -1;
	}
	// printf("\n\n**InfoHeader**\n");
	// printf(" size: %d \n width: %d \n height: %d \n planes: %d \n bits: %d \n",infoHeader.size,infoHeader.width,infoHeader.height,infoHeader.planes,infoHeader.bits); 
	// printf(" compression: %d \n imagesize: %d \n xResolution: %d \n yResolution: %d \n",infoHeader.compression,infoHeader.imagesize,infoHeader.xResolution,infoHeader.yResolution);
	// printf(" coloursUsed: %d \n importantColours: %d \n\n",infoHeader.coloursUsed,infoHeader.importantColours);


	if((image = fopen("image.bmp","wb")) == NULL)
	{
		printf("Error Reading File\n");
		fclose(fp);
		free(bitMapImage);
		fclose(image);
		return -1;
	}

	if((ERROR = fwrite(&header,sizeof(header),1,image)) != 1)
	{
		printf("Error Writing Header\n");
		printf("ERROR: %d \n",ERROR);
		fclose(fp);
		free(bitMapImage);
		fclose(image);
		return -1;
	}

	if(fwrite(&infoHeader,sizeof(infoHeader),1,image) != 1)
	{
		printf("Error Writing InfoHeader\n");
		fclose(fp);
		free(bitMapImage);
		fclose(image);
		return -1;
	}

	//fseek(image,header.offset,SEEK_SET);
	if((ERROR = fwrite(bitMapImage,infoHeader.imagesize,1,image))  != 1) // Read the bit map as one data set
	{
		printf("Error Writing Image BitMap\n");
		printf("ERROR:%d\n",ERROR);
		free(bitMapImage);
		fclose(image);
		return -1;
	}
	for (i = 0; i < infoHeader.imagesize; ++i)
	{
		printf("%d ",bitMapImage[i]);
		if(i%32 == 0)
			printf("\n");
	}

	fclose(image);
	fclose(fp);
	return 0;
}

// another approach save the file as a bmp with width 8 and height 32768 * 3

// width = blockFile height = width / block * height / width * 32768