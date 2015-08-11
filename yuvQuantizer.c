/*
Author - Abel Paul Babu
Roll Number - MT2014501

Code for - 

	Reading BMP file
	Dividing it to blocks

reference -
	 http://paulbourke.net/dataformats/bmp/
	 http://www.ue.eti.pg.gda.pl/fpgalab/zadania.spartan3/zad_vga_struktura_pliku_bmp_en.html

*/


	



#include <stdio.h>
#include <stdlib.h>

#define numColor 3	 

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

// read - http://stackoverflow.com/questions/3318410/pragma-pack-effect


void colourVector2MatrixConverter(unsigned char ** red,unsigned char ** green,unsigned char ** blue,unsigned char * bitMapImage,int height, int width)
{
	int i,j,k;
	for (i = 0,j = 0, k = 0; i < height * width * numColor; i+=3)
	{
			blue[j][k] = bitMapImage[i]; 
			green[j][k] = bitMapImage[i + 1];
			red[j][k] = bitMapImage[i + 2];
			//printf("j:%d k:%d\n",j,k);
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
	for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; ++j)
		{
			Y[i][j] = red[i][j] * 0.2999 + green[i][j] * 0.587 + blue[i][j] * 0.114;  
			if(Y[i][j] > 255)
				Y[i][j] = 255;
			if(Y[i][j] < 0)
				Y[i][j] = 0;

			U[i][j] = red[i][j] * (-0.168736) + green[i][j] * (-0.331264) + blue[i][j] * (0.500002) + 128;
			if(U[i][j] > 255)
				U[i][j] = 255;
			if(U[i][j] < 0)
				U[i][j] = 0;

			V[i][j] = red[i][j] * 0.5 + green[i][j] * (-0.418688) + blue[i][j] * (-0.081312) + 128;
			if(V[i][j] > 255)
				V[i][j] = 255;
			if(V[i][j] < 0)
				V[i][j] = 0; 			
		}
	}

}


void quantiser(unsigned char ** Y,unsigned char ** U,unsigned char ** V,int height, int width)
{
	int i,j;
	for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; ++j)
		{
			U[i][j] = 0;
			V[i][j] = 0;			
		}
	}	
}

void yuv2rgb(unsigned char ** red,unsigned char ** green,unsigned char ** blue, unsigned char ** Y,unsigned char ** U,unsigned char ** V,int height, int width)
{
	int i,j;
	for (i = 0; i < height; ++i)
	{
		for (j = 0; j < width; ++j)
		{
			red[i][j] = Y[i][j] + (V[i][j] - 128 )* 1.4021;  
			if(red[i][j] > 255)
				red[i][j] = 255;
			if(red[i][j] < 0)
				red[i][j] = 0;

			green[i][j] = Y[i][j] + (U[i][j] - 128 ) * (-0.34414) + (V[i][j] - 128) * (-0.71414);  
			if(green[i][j] > 255)
				green[i][j] = 255;
			if(green[i][j] < 0)
				green[i][j] = 0;

			blue[i][j] = Y[i][j] + (U[i][j] - 128) * 1.77180;
			if(blue[i][j] > 255)
				blue[i][j] = 255;
			if(blue[i][j] < 0)
				blue[i][j] = 0; 			
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
			//printf("j:%d k:%d\n",j,k);
			k++;
			if(k == width)
			{
				j++;
				k = 0;
			}
		
	}
}


int main(int argc, char const *argv[])
{
	if(argc != 2)
	{
		printf("\n\nThe format is ./quantizer [file name]\n\n");
		return -1;
	}

	int i,ERROR;
	unsigned char *bitMapImage;
	unsigned char **red,**green,**blue;
	unsigned char **Y,**U,**V;
	FILE *fp,*image;
	Header header;
	InfoHeader infoHeader;

	if((fp = fopen(argv[1],"rb")) == NULL)
	{
		printf("Error Reading File\n");
		fclose(fp);
		return -1;
	}

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
	// printf("**Header**\n");
	// printf(" type: 0x%x \n fileSize: %d \n reserved1: %d \n offset: %d \n",header.type,header.fileSize,header.reserved,header.offset);

	if(fread(&infoHeader,sizeof(infoHeader),1,fp) != 1)
	{
		printf("Error Reading Header\n");
		fclose(fp);
		return -1;
	}
	// printf("\n\n**InfoHeader**\n");
	// printf(" size: %d \n width: %d \n height: %d \n planes: %d \n bits: %d \n",infoHeader.size,infoHeader.width,infoHeader.height,infoHeader.planes,infoHeader.bits); 
	// printf(" compression: %d \n imagesize: %d \n xResolution: %d \n yResolution: %d \n",infoHeader.compression,infoHeader.imagesize,infoHeader.xResolution,infoHeader.yResolution);
	// printf(" coloursUsed: %d \n importantColours: %d \n\n",infoHeader.coloursUsed,infoHeader.importantColours);

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


	
	red = (unsigned char **) malloc(infoHeader.height * sizeof(char*) );
	blue = (unsigned char **) malloc(infoHeader.height * sizeof(char*) );
	green = (unsigned char **) malloc(infoHeader.height * sizeof(char*) );
	Y = (unsigned char **) malloc(infoHeader.height * sizeof(char*) );
	U = (unsigned char **) malloc(infoHeader.height * sizeof(char*) );
	V = (unsigned char **) malloc(infoHeader.height * sizeof(char*) );

	for (i = 0; i < infoHeader.height; ++i)
	{
		red[i] = (unsigned char *) malloc(infoHeader.width * sizeof(char) );
		blue[i] = (unsigned char *) malloc(infoHeader.width * sizeof(char) );
		green[i] = (unsigned char *) malloc(infoHeader.width * sizeof(char) );
		Y[i] = (unsigned char *) malloc(infoHeader.width * sizeof(char) );
		U[i] = (unsigned char *) malloc(infoHeader.width * sizeof(char) );
		V[i] = (unsigned char *) malloc(infoHeader.width * sizeof(char) );		
	}

	colourVector2MatrixConverter(red,green,blue,bitMapImage,infoHeader.width,infoHeader.height);

	rgb2yuv(red,green,blue,Y,U,V,infoHeader.width,infoHeader.height);

	quantiser(Y,U,V,infoHeader.width,infoHeader.height);

	yuv2rgb(red,green,blue,Y,U,V,infoHeader.width,infoHeader.height);


	colourMatrix2VectorConverter(red,green,blue,bitMapImage,infoHeader.width,infoHeader.height);
		
	if((image = fopen("quantized.bmp","wb")) == NULL)
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

	fseek(image,header.offset,SEEK_SET);
	if((ERROR = fwrite(bitMapImage,infoHeader.imagesize,1,image))  != 1) // Read the bit map as one data set
	{
		printf("Error Writing Image BitMap\n");
		printf("ERROR:%d\n",ERROR);
		free(bitMapImage);
		fclose(image);
		return -1;
	}




	free(red);
	free(green);
	free(blue);
	free(bitMapImage);
	return 0;
}

// `./rgb2yvu images/Lenna.bmp 			