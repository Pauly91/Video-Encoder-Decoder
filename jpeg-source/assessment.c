#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bmpReader.h"

unsigned char luminanceQuantizationMatrixData[8][8] = {
				{16,11,	10,	16,	24,	40,	51,	 61},
				{12,12,	14,	19,	26,	58,	60,	 55},
				{14,13,	16,	24,	40,	57,	69,	 56},
				{14,17,	22,	29,	51,	87,	80,	 62},
				{18,22,	37,	56,	68,	109,103, 77},
				{24,35,	55,	64,	81,	104,113, 92},
				{49,64,	78,	87,	103,121,120,101},
				{72,92,	95,	98,	112,100,103, 99}
};

// unsigned char luminanceQuantizationMatrixData[8][8] = {
// 						{20,14,	13,	20,	30,	50,	64,	76},
// 						{15,15,	18,	24,	33,	73,	75,	69},
// 						{18,16,	20,	30,	50,	71,	86,	70},
// 						{18,21,	28,	36,	64,	109,100,78},
// 						{23,28,	46,	70,	85,	136,129,96},
// 						{30,44,	69,	80,	101,130,141,115},
// 						{61,80,	98,	109,129,151,150,126},
// 						{90,115,119,123,140,125,129,124}
// };


unsigned char chrominanceQuantizationMatrixData[8][8] = {
				{17,18,	24,	47,	99,	99,	99,	99},
				{18,21,	26,	66,	99,	99,	99,	99},
				{24,26,	56,	99,	99,	99,	99,	99},
				{47,66,	99,	99,	99,	99,	99,	99},
				{99,99,	99,	99,	99,	99,	99,	99},
				{99,99,	99,	99,	99,	99,	99,	99},
				{99,99,	99,	99,	99,	99,	99,	99},
				{99,99,	99,	99,	99,	99,	99,	99}
};

// unsigned char chrominanceQuantizationMatrixData[8][8] = {
// 						{21 ,23 ,30	,59	,124,124,124,124},
// 						{23 ,26 ,33	,83	,124,124,124,124},
// 						{30 ,33 ,70	,124,124,124,124,124},
// 						{59 ,83 ,124,124,124,124,124,124},
// 						{124,124,124,124,124,124,124,124},
// 						{124,124,124,124,124,124,124,124},
// 						{124,124,124,124,124,124,124,124},
// 						{124,124,124,124,124,124,124,124}
// };

int main(int argc, char const *argv[])
{
	if(argc != 4)
	{
		printf("\n ./run [file name] [blockSize] [type of downSampling - 444 422 411 421 420] \n\n");
		return -1;
	}
	BMPData *image = NULL;

	int i,j,l,k;
	int returnScanf = 0;
	int blockSize = atoi(argv[2]);
	if(blockSize != 8)
	{
		printf("Only 8*8 quantization block exist\n");
		printf("Continue with block size of 8*8 ? Y or N \n");
		//getc(question); // complete this question
	}
	int dSampleU = 0;
	int dSampleV = 0;
	int DsampleType = 0	;
	int numberOfBlockY_x = 0;
	int numberOfBlockY_y = 0;
	int numberOfBlockUV_x = 0;
	int numberOfBlockUV_y = 0;
	int dSampleHeight;
	int dSampleWidth;
	int dSampleBlockWidth = 4;
	int dSampleBlockHeight = 2;

	DsampleType = atoi(argv[3]);
	dSampleV = DsampleType%10;
	DsampleType /= 10;
	dSampleU = DsampleType%10;
	printf("dSampleU:%d dSampleV:%d\n",dSampleU,dSampleV);		

	unsigned char **red = NULL,**green = NULL,**blue = NULL;
	unsigned char **Y = NULL,**U = NULL,**V = NULL;
	unsigned char **downSampledU = NULL,**downSampledV = NULL;
	unsigned char **luminanceQuantizationMatrix = NULL;
	unsigned char **chrominanceQuantizationMatrix = NULL;


	luminanceQuantizationMatrix = (unsigned char **) malloc(blockSize * sizeof(char*) );
	chrominanceQuantizationMatrix = (unsigned char **) malloc(blockSize * sizeof(char*) );


	float **dct_Y = NULL,**dct_U = NULL,**dct_V = NULL;//**dct_U_full = NULL,**dct_V_full = NULL;
	int **dct_Y_decode = NULL,**dct_U_decode = NULL,**dct_V_decode = NULL;
	FILE *fp;

	for (i = 0; i < blockSize; ++i)
	{
		luminanceQuantizationMatrix [i] = (unsigned char *) malloc(blockSize * sizeof(char) );
		chrominanceQuantizationMatrix[i] = (unsigned char *) malloc(blockSize * sizeof(char) );
	}	
	for (i = 0; i < blockSize; ++i)
	{
		for (j = 0; j < blockSize; ++j)
		{
			luminanceQuantizationMatrix[i][j] = luminanceQuantizationMatrixData[i][j];
			chrominanceQuantizationMatrix[i][j] = chrominanceQuantizationMatrixData[i][j];
		}
	}

	if((image = readBMPfile(argv[1])) == NULL)
		printf("Error in File 1\n");	

	numberOfBlockY_x = image->infoHeader.height / blockSize;
	numberOfBlockY_y = image->infoHeader.width / blockSize;


	red = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );
	blue = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );
	green = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );

	Y = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );
	U = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );
	V = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );

	dct_Y = (float **) malloc(image->infoHeader.height * sizeof(float*) );

	// dct_U_full = (float **) malloc(image->infoHeader.height * sizeof(float*) );
	// dct_V_full = (float **) malloc(image->infoHeader.height * sizeof(float*) );
	dct_Y_decode = (int **) malloc(image->infoHeader.height * sizeof(int*) );


	for (i = 0; i < image->infoHeader.height; ++i)
	{
		red[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );
		blue[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );
		green[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );

		Y[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );
		U[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );
		V[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );	

		dct_Y[i] = (float  *) malloc(image->infoHeader.width * sizeof(float) );
		// dct_U_full[i] = (float  *) malloc(image->infoHeader.width * sizeof(float) );
		// dct_V_full[i] = (float  *) malloc(image->infoHeader.width * sizeof(float) );				
		dct_Y_decode[i] = (int  *) malloc(image->infoHeader.width * sizeof(int) );


	}


	if(dSampleU == 1 && dSampleV == 1)
	{
		dSampleWidth = image->infoHeader.width/4;
		dSampleHeight = image->infoHeader.height;
	}
	else if(dSampleU == 2 && dSampleV == 0)
	{
		dSampleWidth = image->infoHeader.width/2;
		dSampleHeight = image->infoHeader.height/2;
	}
	else if(dSampleU == 4 && dSampleV == 0)
	{
		dSampleWidth = image->infoHeader.width;
		dSampleHeight = image->infoHeader.height/2;
	}
	else
	{
		dSampleWidth = image->infoHeader.width;
		dSampleHeight = image->infoHeader.height;		
	}

	numberOfBlockUV_x = dSampleHeight / blockSize;
	numberOfBlockUV_y = dSampleWidth / blockSize;

	printf("dSampleHeight:%d dSampleWidth:%d\n",dSampleHeight,dSampleWidth);
	downSampledU = (unsigned char **) malloc(dSampleHeight * sizeof(char*) );
	downSampledV = (unsigned char **) malloc(dSampleHeight * sizeof(char*) );

	dct_U = (float **) malloc(dSampleHeight * sizeof(float*) );
	dct_V = (float **) malloc(dSampleHeight * sizeof(float*) );
	dct_U_decode = (int **) malloc(dSampleHeight * sizeof(int*) );
	dct_V_decode = (int **) malloc(dSampleHeight * sizeof(int*) );

	for (i = 0; i < dSampleHeight; ++i)
	{
		downSampledU[i] = (unsigned char *) malloc(dSampleWidth * sizeof(char));
		downSampledV[i] = (unsigned char *) malloc(dSampleWidth * sizeof(char));	
		dct_U[i] = (float  *) malloc(dSampleWidth  * sizeof(float) );
		dct_V[i] = (float  *) malloc(dSampleWidth  * sizeof(float) );	
		dct_U_decode[i] = (int  *) malloc(dSampleWidth  * sizeof(int) );
		dct_V_decode[i] = (int  *) malloc(dSampleWidth  * sizeof(int) );	
	}

	colourVector2MatrixConverter(red,green,blue,image->bitMapImage,image->infoHeader.height,image->infoHeader.width);
	
	rgb2yuv(red,green,blue,Y,U,V,image->infoHeader.height,image->infoHeader.width);




////////////////////////////////////////////////////////////////////////
	// if((fp = fopen("YUV","w")) == NULL)
	// {
	// 	printf("YUV file not opened\n");
	// 	return -1;
	// }

	// for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	// {
	// 	for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
	// 	{

	// 		fprintf(fp, "%u%u%u",Y[i][j],U[i][j],V[i][j]);

	// 	}
	// }
	// fclose(fp);


	if((fp = fopen("YUV","w")) == NULL)
	{
		printf("YUV file not opened\n");
		return -1;
	}

	for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%u ",Y[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%u ",U[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%u ",V[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}		
	fclose(fp);		

////////////////////////////////////////////////////////////////////////

	DCT(image, dct_Y, dct_U, dct_V, Y, U, V, blockSize,  dSampleHeight, dSampleWidth);

////////////////////////////////////////////////////////////////////////
	// if((fp = fopen("DCT_beforeDownSampling","w")) == NULL)
	// {
	// 	printf("DCT_beforeDownSampling not opened\n");
	// 	return -1;
	// }

	// for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	// {
	// 	for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
	// 	{
	// 		 for (l = 0; l < blockSize; ++l)
	// 		 {
	// 		 	for (k = 0; k < blockSize; ++k)
	// 		 	{
	// 				fprintf(fp, "%f ",dct_Y[i + l][j + k]);
	// 		 	}
	// 		 	fprintf(fp,"\n");	
	// 		 }
	// 		 fprintf(fp, "\n");
	// 		 for (l = 0; l < blockSize; ++l)
	// 		 {
	// 		 	for (k = 0; k < blockSize; ++k)
	// 		 	{
	// 				fprintf(fp, "%f ",dct_U_full[i + l][j + k]);
	// 		 	}
	// 		 	fprintf(fp,"\n");	
	// 		 }
	// 		 fprintf(fp, "\n");		
	// 		 for (l = 0; l < blockSize; ++l)
	// 		 {
	// 		 	for (k = 0; k < blockSize; ++k)
	// 		 	{
	// 				fprintf(fp, "%f ",dct_V_full[i + l][j + k]);
	// 		 	}
	// 		 	fprintf(fp,"\n");	
	// 		 }
	// 		 fprintf(fp, "\n");
	// 	}
	// }
	// fclose(fp);
////////////////////////////////////////////////////////////////////////


	downSample(image, Y, U, V, downSampledU, downSampledV, dSampleU, dSampleV); // CHeck the loop in the definition of this function


////////////////////////////////////////////////////////////////////////

	// if((fp = fopen("YUV_downSampled","w")) == NULL)
	// {
	// 	printf("dct file not opened\n");
	// 	return -1;
	// }

	// for (i = 0; i < dSampleHeight; ++i) // check loop
	// {
	// 	for (j = 0; j < dSampleWidth; ++j) // check loop
	// 	{
	// 		 for (l = 0; l < dSampleU; ++l)
	// 		 {
	// 		 	for (k = 0; k < dSampleU; ++k)
	// 		 	{
	// 				fprintf(fp, "%u ",Y[i + l][j + k]);
	// 		 	}
	// 		 }
	// 		 fprintf(fp, "%u %u \n",downSampledU[i][j],downSampledV[i][j]);
	// 	}
	// }

	// fclose(fp);


	if((fp = fopen("YUV_downSampled","w")) == NULL)
	{
		printf("dct file not opened\n");
		return -1;
	}

	for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%d ",Y[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	{
		for (j = 0; j < dSampleWidth; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%d ",downSampledU[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	{
		for (j = 0; j < dSampleWidth; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%d ",downSampledV[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	fclose(fp);	

////////////////////////////////////////////////////////////////////////

	DCT(image, dct_Y, dct_U, dct_V, Y, downSampledU, downSampledV, blockSize, dSampleHeight, dSampleWidth);

////////////////////////////////////////////////////////////////////////

	// if((fp = fopen("DCT_afterDownSampling","w")) == NULL)
	// {
	// 	printf("dct file not opened\n");
	// 	return -1;
	// }

	// for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	// {
	// 	for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
	// 	{
	// 		 for (l = 0; l < blockSize; ++l)
	// 		 {
	// 		 	for (k = 0; k < blockSize; ++k)
	// 		 	{
	// 				fprintf(fp, "%f ",dct_Y[i + l][j + k]);
	// 		 	}
	// 		 }
	// 		 fprintf(fp, "\n");
	// 	}
	// }

	// for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	// {
	// 	for (j = 0; j < dSampleWidth; j += blockSize) // check loop
	// 	{
	// 		 for (l = 0; l < blockSize; ++l)
	// 		 {
	// 		 	for (k = 0; k < blockSize; ++k)
	// 		 	{
	// 				fprintf(fp, "%f ",dct_U[i + l][j + k]);
	// 		 	}
	// 		 }
	// 		 fprintf(fp, "\n");
	// 	}
	// }

	// for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	// {
	// 	for (j = 0; j < dSampleWidth; j += blockSize) // check loop
	// 	{
	// 		 for (l = 0; l < blockSize; ++l)
	// 		 {
	// 		 	for (k = 0; k < blockSize; ++k)
	// 		 	{
	// 				fprintf(fp, "%f ",dct_V[i + l][j + k]);
	// 		 	}
	// 		 }
	// 		 fprintf(fp, "\n");
	// 	}
	// }

	// fclose(fp);


	if((fp = fopen("DCT_afterDownSampling","w")) == NULL)
	{
		printf("dct file not opened\n");
		return -1;
	}

	for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%f ",dct_Y[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	{
		for (j = 0; j < dSampleWidth; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%f ",dct_U[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	{
		for (j = 0; j < dSampleWidth; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%f ",dct_V[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	fclose(fp);
////////////////////////////////////////////////////////////////////////


	quantize(image,dct_Y, dct_U, dct_V, luminanceQuantizationMatrix, chrominanceQuantizationMatrix, blockSize, dSampleHeight, dSampleWidth);

////////////////////////////////////////////////////////////////////////

	// if((fp = fopen("DCT_afterQuantizingSampling","w")) == NULL)
	// {
	// 	printf("dct file not opened\n");
	// 	return -1;
	// }

	// for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	// {
	// 	for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
	// 	{
	// 		 for (l = 0; l < blockSize; ++l)
	// 		 {
	// 		 	for (k = 0; k < blockSize; ++k)
	// 		 	{
	// 				fprintf(fp, "%f ",dct_Y[i + l][j + k]);
	// 		 	}
	// 		 }
	// 		 fprintf(fp, "\n");
	// 	}
	// }

	// for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	// {
	// 	for (j = 0; j < dSampleWidth; j += blockSize) // check loop
	// 	{
	// 		 for (l = 0; l < blockSize; ++l)
	// 		 {
	// 		 	for (k = 0; k < blockSize; ++k)
	// 		 	{
	// 				fprintf(fp, "%f ",dct_U[i + l][j + k]);
	// 		 	}
	// 		 }
	// 		 fprintf(fp, "\n");
	// 	}
	// }

	// for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	// {
	// 	for (j = 0; j < dSampleWidth; j += blockSize) // check loop
	// 	{
	// 		 for (l = 0; l < blockSize; ++l)
	// 		 {
	// 		 	for (k = 0; k < blockSize; ++k)
	// 		 	{
	// 				fprintf(fp, "%f ",dct_V[i + l][j + k]);
	// 		 	}
	// 		 }
	// 		 fprintf(fp, "\n");
	// 	}
	// }

	// fclose(fp);


	if((fp = fopen("DCT_afterQuantizingSampling","w")) == NULL)
	{
		printf("dct file not opened\n");
		return -1;
	}

	for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%d ",(int) dct_Y[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	{
		for (j = 0; j < dSampleWidth; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%d ",(int) dct_U[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	{
		for (j = 0; j < dSampleWidth; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%d ",(int) dct_V[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	fclose(fp);


////////////////////////////////////////////////////////////////////////

	
	zigzag(image, dct_Y, blockSize,"zigzag_DCT_Y", 0, 0); // not downsampling for Y
	zigzag(image, dct_U, blockSize,"zigzag_DCT_U", dSampleHeight, dSampleWidth);
	zigzag(image, dct_V, blockSize,"zigzag_DCT_V", dSampleHeight, dSampleWidth);


	differentialHuffmanRle("zigzag_DCT_Y","encoded_Y","Y_Byte_data",blockSize * blockSize);
	differentialHuffmanRle("zigzag_DCT_U","encoded_U","U_Byte_data",blockSize * blockSize);
	differentialHuffmanRle("zigzag_DCT_V","encoded_V","V_Byte_data",blockSize * blockSize);


	free(dct_U);
	free(dct_V);
	free(dct_Y_decode);
	free(dct_U_decode);
	free(dct_V_decode);
	free(Y);
	free(U);
	free(V);
	free(downSampledU);
	free(downSampledV);
	free(red);
	free(green);
	free(blue);
	

	dct_U = (float **) malloc(dSampleHeight * sizeof(float*) );
	dct_V = (float **) malloc(dSampleHeight * sizeof(float*) );
	dct_U_decode = (int **) malloc(dSampleHeight * sizeof(int*) );
	dct_V_decode = (int **) malloc(dSampleHeight * sizeof(int*) );

	for (i = 0; i < dSampleHeight; ++i)
	{
		downSampledU[i] = (unsigned char *) malloc(dSampleWidth * sizeof(char));
		downSampledV[i] = (unsigned char *) malloc(dSampleWidth * sizeof(char));	
		dct_U[i] = (float  *) malloc(dSampleWidth  * sizeof(float) );
		dct_V[i] = (float  *) malloc(dSampleWidth  * sizeof(float) );	
		dct_U_decode[i] = (int  *) malloc(dSampleWidth  * sizeof(int) );
		dct_V_decode[i] = (int  *) malloc(dSampleWidth  * sizeof(int) );	
	}
	red = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );
	blue = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );
	green = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );

	Y = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );
	U = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );
	V = (unsigned char **) malloc(image->infoHeader.height * sizeof(char*) );

	dct_Y = (float **) malloc(image->infoHeader.height * sizeof(float*) );


	dct_Y_decode = (int **) malloc(image->infoHeader.height * sizeof(int*) );


	for (i = 0; i < image->infoHeader.height; ++i)
	{
		red[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );
		blue[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );
		green[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );

		Y[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );
		U[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );
		V[i] = (unsigned char *) malloc(image->infoHeader.width * sizeof(char) );	

		dct_Y[i] = (float  *) malloc(image->infoHeader.width * sizeof(float) );
		dct_Y_decode[i] = (int  *) malloc(image->infoHeader.width * sizeof(int) );


	}
	decodeRleHuffman("Y_Byte_data","decodeZigZagY", numberOfBlockY_x, numberOfBlockY_y, blockSize * blockSize);
	decodeRleHuffman("U_Byte_data","decodeZigZagU", numberOfBlockUV_x, numberOfBlockUV_y, blockSize * blockSize);
	decodeRleHuffman("V_Byte_data","decodeZigZagV", numberOfBlockUV_x, numberOfBlockUV_y, blockSize * blockSize);

	reAssembleZigZag("decodeZigZagY", dct_Y_decode, image->infoHeader.height, image->infoHeader.width, blockSize);
	reAssembleZigZag("decodeZigZagU", dct_U_decode, dSampleHeight, dSampleWidth, blockSize);
	reAssembleZigZag("decodeZigZagV", dct_V_decode, dSampleHeight, dSampleWidth, blockSize);

	if((fp = fopen("Decode_DCT_afterQuantizingSampling","w")) == NULL)
	{
		printf("dct file not opened\n");
		return -1;
	}
	fprintf(fp,"Y\n");
	for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%d ",dct_Y_decode[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}
	fprintf(fp,"U\n");
	for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	{
		for (j = 0; j < dSampleWidth; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%d ",dct_U_decode[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}
	fprintf(fp,"V\n");
	for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	{
		for (j = 0; j < dSampleWidth; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%d ",dct_V_decode[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	fclose(fp);

	deQuantize(image,dct_Y_decode, dct_U_decode, dct_V_decode, luminanceQuantizationMatrix, chrominanceQuantizationMatrix, blockSize, dSampleHeight, dSampleWidth);


	// if((fp = fopen("Decode_DCT_afterDownSampling","w")) == NULL)
	// {
	// 	printf("dct file not opened\n");
	// 	return -1;
	// }

	// for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	// {
	// 	for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
	// 	{
	// 		 for (l = 0; l < blockSize; ++l)
	// 		 {
	// 		 	for (k = 0; k < blockSize; ++k)
	// 		 	{
	// 				fprintf(fp, "%f ",dct_Y_decode[i + l][j + k]);
	// 		 	}
	// 		 }
	// 		 fprintf(fp, "\n");
	// 	}
	// }

	// for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	// {
	// 	for (j = 0; j < dSampleWidth; j += blockSize) // check loop
	// 	{
	// 		 for (l = 0; l < blockSize; ++l)
	// 		 {
	// 		 	for (k = 0; k < blockSize; ++k)
	// 		 	{
	// 				fprintf(fp, "%f ",dct_U_decode[i + l][j + k]);
	// 		 	}
	// 		 }
	// 		 fprintf(fp, "\n");
	// 	}
	// }

	// for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	// {
	// 	for (j = 0; j < dSampleWidth; j += blockSize) // check loop
	// 	{
	// 		 for (l = 0; l < blockSize; ++l)
	// 		 {
	// 		 	for (k = 0; k < blockSize; ++k)
	// 		 	{
	// 				fprintf(fp, "%f ",dct_V_decode[i + l][j + k]);
	// 		 	}
	// 		 }
	// 		 fprintf(fp, "\n");
	// 	}
	// }

	// fclose(fp);


	if((fp = fopen("Decode_DCT_afterDownSampling","w")) == NULL)
	{
		printf("dct file not opened\n");
		return -1;
	}
	fprintf(fp,"Y\n");
	for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%d ",dct_Y_decode[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}
	fprintf(fp,"U\n");
	for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	{
		for (j = 0; j < dSampleWidth; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%d ",dct_U_decode[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}
	fprintf(fp,"V\n");
	for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	{
		for (j = 0; j < dSampleWidth; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%d ",dct_V_decode[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	fclose(fp);

	IDCT(image, dct_Y_decode, dct_U_decode, dct_V_decode, Y, downSampledU, downSampledV, blockSize, dSampleHeight, dSampleWidth);


	// if((fp = fopen("decode_YUV_downSampled","w")) == NULL)
	// {
	// 	printf("dct file not opened\n");
	// 	return -1;
	// }

	// for (i = 0; i < dSampleHeight; ++i) // check loop
	// {
	// 	for (j = 0; j < dSampleWidth; ++j) // check loop
	// 	{
	// 		 for (l = 0; l < dSampleU; ++l)
	// 		 {
	// 		 	for (k = 0; k < dSampleU; ++k)
	// 		 	{
	// 				fprintf(fp, "%u ",Y[i + l][j + k]);
	// 		 	}
	// 		 }
	// 		 fprintf(fp, "%u %u \n",downSampledU[i][j],downSampledV[i][j]);
	// 	}
	// }

	// fclose(fp);


	if((fp = fopen("decode_YUV_downSampled","w")) == NULL)
	{
		printf("dct file not opened\n");
		return -1;
	}

	for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%d ",Y[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	{
		for (j = 0; j < dSampleWidth; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%d ",downSampledU[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	for (i = 0; i < dSampleHeight; i += blockSize) // check loop
	{
		for (j = 0; j < dSampleWidth; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%d ",downSampledV[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	fclose(fp);	

	upSample(image, Y, U, V, downSampledU, downSampledV, dSampleU, dSampleV, dSampleHeight, dSampleWidth);
	
	// if((fp = fopen("decode_YUV","w")) == NULL)
	// {
	// 	printf("YUV file not opened\n");
	// 	return -1;
	// }

	// for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	// {
	// 	for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
	// 	{

	// 		fprintf(fp, "%u%u%u",Y[i][j],U[i][j],V[i][j]);

	// 	}
	// }
	// fclose(fp);
	if((fp = fopen("decode_YUV","w")) == NULL)
	{
		printf("YUV file not opened\n");
		return -1;
	}

	for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%u ",Y[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%u ",U[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}

	for (i = 0; i < image->infoHeader.height; i += blockSize) // check loop
	{
		for (j = 0; j < image->infoHeader.width; j += blockSize) // check loop
		{
			 for (l = 0; l < blockSize; ++l)
			 {
			 	for (k = 0; k < blockSize; ++k)
			 	{
					fprintf(fp, "%u ",V[i + l][j + k]);
			 	}
			 	fprintf(fp,"\n");	
			 }
			 fprintf(fp, "\n");
		}
	}		
	fclose(fp);		

	yuv2rgb(red, green, blue, Y, U, V, image->infoHeader.height, image->infoHeader.width);
	colourMatrix2VectorConverter(red,green,blue,image->bitMapImage,image->infoHeader.height,image->infoHeader.width);
	if(writeBMPfile(image,"decodeBMP.bmp") < 0)
	{
		printf("Error Writing decoded BMP file\n");
		return -1;
	}


/*
DPCM with the huffman code is issue

*/
	return 0;
}