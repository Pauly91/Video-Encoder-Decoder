/*

Header file to read a BMP file

returns a structure


	struct BMP_data
	{
		struct header;
		struct infoHeader;
		char *array 
	}


*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>	

#define numColor 3
#define pi 3.145
#define DCtableSize 12
#define ACtableSize 162
#define MaxPixel 64
#define byteSize 8
#define dataSize 64

#pragma pack(push, 1)	


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

#pragma pack(pop) 

typedef struct BMPDatas
{
	Header header;
	InfoHeader infoHeader;
	unsigned char *bitMapImage;

} BMPData;



BMPData* readBMPfile(const char *filename);
int writeBMPfile(BMPData *data,const char *filename);


void colourVector2MatrixConverter(unsigned char ** red,unsigned char ** green,unsigned char ** blue,unsigned char * bitMapImage,int height, int width);
void colourMatrix2VectorConverter(unsigned char ** red,unsigned char ** green,unsigned char ** blue,unsigned char * bitMapImage,int height, int width);

void rgb2yuv(unsigned char ** red,unsigned char ** green,unsigned char ** blue,unsigned char ** Y,unsigned char ** U,unsigned char ** V,int height, int width);
void yuv2rgb(unsigned char ** red,unsigned char ** green,unsigned char ** blue,unsigned char ** Y,unsigned char ** U,unsigned char ** V,int height, int width);

void DCT(BMPData *image, float **dct_Y, float **dct_U, float **dct_V, unsigned char ** Y, unsigned char ** U, unsigned char ** V, int blockSize, int dSampleHeight, int dSampleWidth);
void downSample(BMPData *image, unsigned char ** Y, unsigned char ** U, unsigned char ** V, unsigned char **downSampledU, unsigned char ** downSampledV, int dSampleU, int dSampleV);
void quantize(BMPData *image, float **dct_Y, float **dct_U, float **dct_V, unsigned char ** luminanceQuantizationMatrix, unsigned char ** chrominanceQuantizationMatrix, int blockSize, int dSampleHeight, int dSampleWidth);
void zigzag(BMPData *image, float **dct,int blockSize, char *filename,int dSampleHeight, int dSampleWidth);
void differentialHuffmanRle(char *zigZagData, char * targetFile,char *byteData);
void decodeRleHuffman(char * byteFile, char *decodeZigZag, int numberOfBlockHeight, int numberOfBlockWidth);
void reAssembleZigZag(char * zigZagFile, int **reblock, int numberOfBlockHeight, int numberOfBlockWidth, int blockSize);
void deQuantize(BMPData *image, int **dct_Y, int **dct_U, int **dct_V, unsigned char ** luminanceQuantizationMatrix, unsigned char ** chrominanceQuantizationMatrix, int blockSize, int dSampleHeight, int dSampleWidth);
void IDCT(BMPData *image, int **dct_Y, int **dct_U, int **dct_V, unsigned char ** Y, unsigned char ** U, unsigned char ** V, int blockSize, int dSampleHeight, int dSampleWidth);
void upSample(BMPData *image, unsigned char ** Y, unsigned char ** U, unsigned char ** V, unsigned char **downSampledU, unsigned char ** downSampledV, int dSampleU, int dSampleV, int dSampleHeight, int dSampleWidth);
