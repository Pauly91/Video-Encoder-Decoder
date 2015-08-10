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
	unsigned char *red,*green,*blue,ignore;
	int block,height,width;
	FILE *fp;
	Header header;
	InfoHeader infoHeader;

	//initHeaders(&header,&infoHeader);

	red = (unsigned char *) calloc(height * width , sizeof(char));
	green = (unsigned char *) calloc(height * width , sizeof(char));
	blue = (unsigned char *) calloc(height * width , sizeof(char));

	if((fp = fopen("blockFile","r")) == NULL)
	{
		printf("Error Reading File\n");
		fclose(fp);
		return -1;
	}

	fscanf(fp,"%d %d %d",&block,&height,&width);
	printf("%d %d %d\n",block,height,width);

	red = (unsigned char *) calloc(height * width , sizeof(char));
	green = (unsigned char *) calloc(height * width , sizeof(char));
	blue = (unsigned char *) calloc(height * width , sizeof(char));

	for (i = 0; i < height/block; i++)
	{
		for (j = 0; j < width ; j++) // check if its block * block
		{
			
			for (k = 0, l = 0; k < block  ; ++k, ++l)
			{	
				fscanf(fp,"%d ",&red[k + width * j  + i * block]);
				//printf("%d\n",k + width * j  + i * block);
				printf("%d ",red[k + width * j  + i * block ]);
			}
			printf("\n");
			//printf("fptr: %d\n",block * j + i);			
		}
		//printf("\n");

	}

	fclose(fp);
	// for (i = 0; i < height; i+=block)
	// {
	// 	for (j = 0; j < width; j+= block) // check if its block * block
	// 	{
			
	// 		for (k = 0, l = 0; k < block  ; ++k, ++l)
	// 		{	
	// 			fscanf(fp,"%c ",&green[k + i * block + j * block * block]);
	// 			printf("%d ",green[k + i * block + j * block * block]);
	// 		}
	// 		printf("\n");
	// 		//printf("fptr: %d\n",block * j + i);			
	// 	}
	// 	//fprintf(blockFile,"\n");

	// }


	// for (i = 0; i < height; i+=block)
	// {
	// 	for (j = 0; j < width; j+= block) // check if its block * block
	// 	{
			
	// 		for (k = 0, l = 0; k < block  ; ++k, ++l)
	// 		{	
	// 			fscanf(fp,"%c ",&blue[k + i * block + j * block * block]);
	// 			printf("%d ",blue[k + i * block + j * block * block]);
	// 		}
	// 		printf("\n");
	// 		//printf("fptr: %d\n",block * j + i);			
	// 	}
	// 	//fprintf(blockFile,"\n");

	// }



/*
	if(fread(&header,sizeof(header),1,fp) != 1)
	{
		printf("Error Reading Header\n");
		fclose(fp);
		return -1;
	}

	if (header.type != 19778)
	{
		printf("Not a BMP file\n");
		fclose(fp);
		return -1;
	}
	printf("**Header**\n");
	printf(" type: 0x%x \n fileSize: %d \n reserved1: %d \n offset: %d \n",header.type,header.fileSize,header.reserved,header.offset);

	if(fread(&infoHeader,sizeof(infoHeader),1,fp) != 1)
	{
		printf("Error Reading Header\n");
		fclose(fp);
		return -1;
	}
	printf("\n\n**InfoHeader**\n");
	printf(" size: %d \n width: %d \n height: %d \n planes: %d \n bits: %d \n",infoHeader.size,infoHeader.width,infoHeader.height,infoHeader.planes,infoHeader.bits); 
	printf(" compression: %d \n imagesize: %d \n xResolution: %d \n yResolution: %d \n",infoHeader.compression,infoHeader.imagesize,infoHeader.xResolution,infoHeader.yResolution);
	printf(" coloursUsed: %d \n importantColours: %d \n\n",infoHeader.coloursUsed,infoHeader.importantColours);

	bitMapImage = (unsigned char *) malloc(infoHeader.imagesize * sizeof(char));

	fseek(fp,header.offset,SEEK_SET);
	if((ERROR = fread(bitMapImage,infoHeader.imagesize,1,fp))  != 1) // Read the bit map as one data set
	{
		printf("Error Reading Image BitMap\n");
		printf("ERROR:%d\n",ERROR);
		free(bitMapImage);
		fclose(fp);
		return -1;
	}
	fclose(fp);
	red = (unsigned char *) malloc(infoHeader.imagesize * sizeof(char) / 3);
	green = (unsigned char *) malloc(infoHeader.imagesize * sizeof(char) / 3);
	blue = (unsigned char *) malloc(infoHeader.imagesize * sizeof(char) / 3);

	for (i = 0,j = 0; i < infoHeader.imagesize; i += 3, ++j )
	{
		red[j] = bitMapImage[i];
		green[j] = bitMapImage[i + 1];
		blue[j] = bitMapImage[i + 2];
		//printf("%d %d %d count : %d\n",red[j],green[j],blue[j],i);

		// check if RGB alignment is correct
	}

	blockFile = fopen("blockFile","w");

	for (i = 0; i < infoHeader.height; i+=block)
	{
		for (j = 0; j < infoHeader.width; j+= block) // check if its block * block
		{
			
			for (k = 0, l = 0; k < block  ; ++k, ++l)
				fprintf(blockFile,"%d ",red[k + i * block + j * block * block]);

			fprintf(blockFile,"\n");
			//printf("fptr: %d\n",block * j + i);			
		}
		//fprintf(blockFile,"\n");

	}

	for (i = 0; i < infoHeader.height; i+=block)
	{
		for (j = 0; j < infoHeader.width; j+= block) // check if its block * block
		{
			
			for (k = 0, l = 0; k < block  ; ++k, ++l)
				fprintf(blockFile,"%d ",green[k + i * block + j * block * block]);

			fprintf(blockFile,"\n");
			//printf("fptr: %d\n",block * j + i);

			
		}
		//fprintf(blockFile,"\n");

	}


	for (i = 0; i < infoHeader.height; i+=block)
	{
		for (j = 0; j < infoHeader.width; j+= block) // check if its block * block
		{
			
			for (k = 0, l = 0; k < block  ; ++k, ++l)
				fprintf(blockFile,"%d ",blue[k + i * block + j * block * block]);

			fprintf(blockFile,"\n");
			//printf("fptr: %d\n",block * j + i);
			
		}
		//fprintf(blockFile,"\n");

	}

	fclose(blockFile);

	free(red);
	free(green);
	free(blue);
	free(bitMapImage);
	*/
	return 0;
}

// ./imageBlocking images/Lenna.bmp 8