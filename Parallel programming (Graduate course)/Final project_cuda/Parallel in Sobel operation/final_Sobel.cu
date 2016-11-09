#include "function.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#define WIDTHBYTES(bits) (((bits)+31)/32*4)
#pragma warning(disable:4996)

#define TRUE 1
#define FALSE 0

__global__ void update(BYTE *NewpGryImg, long *LineBytes,long *height, long *width, BYTE *pGryImg, float *tempArray)
{
	long id = threadIdx.x;
	id += (blockIdx.x * 512);
	if(id >= 0 && id < (((*height) - 3 + 1)*((*width) - 3 + 1)))
	{
		long temp = (*width) - 3 + 1;
		long id_i = id / temp;
		id_i += 1;
		long id_j = id % temp;
		id_j += 1;
		BYTE *lpSrc;
		BYTE *lpDst;
		lpDst = NewpGryImg + (*LineBytes)*((*height) - 1 - id_i) + id_j;
		float result = 0;
		for (long k = 0; k < 3; k++)
		{
			for (long l = 0; l < 3; l++)
			{
				lpSrc = pGryImg + (*LineBytes)*((*height) - 1 - id_i + 1 - k) + id_j - 1 + l;
				result += (*lpSrc)*tempArray[k*3 + l];
			}
		}
		result = result*(1.0);
		result = (float)fabs(result);
		if (result > 255)
		{
			*lpDst = 255;
		}
		else
		{
			*lpDst = (unsigned char)(result + 0.5);
		}
	}
}

bool Sobel(BYTE *pGryImg, int width, int height, int threshold);

BYTE *Read8BitBmpFile2Img(const char* filename, int *width, int *height)
{
	FILE *BinFile;
	BITMAPFILEHEADER FileHeader;
	BITMAPINFOHEADER BmpHeader;
	BYTE *plmg;
	unsigned int size;
	int Suc = 1, w, h;

	//open file
	*width = *height = 0;
	if ((BinFile = fopen(filename, "rb")) == NULL) return NULL;
	//read struct info
	if (fread((void*)&FileHeader, 1, sizeof(FileHeader), BinFile) != sizeof(FileHeader)) Suc = -1;
	if (fread((void*)&BmpHeader, 1, sizeof(BmpHeader), BinFile) != sizeof(BmpHeader)) Suc = -1;
	if ((Suc == -1) || (FileHeader.bfOffBits<sizeof(FileHeader)+sizeof(BmpHeader)))
	{
		fclose(BinFile);
		return NULL;
	}
	//read Image data
	*width = w = BmpHeader.biWidth;
	*height = h = BmpHeader.biHeight;
	size = w*h;
	fseek(BinFile, FileHeader.bfOffBits, SEEK_SET);
	if ((plmg = (BYTE*)malloc(sizeof(BYTE)*size)) != NULL)
	{
		for (int i = 0; i<h; i++)  //0,1,2,3,4(5);400-499
		{
			if (fread(plmg + (h - 1 - i)*w, sizeof(BYTE), w, BinFile) != w)
			{
				fclose(BinFile);
				free( plmg);
				plmg = NULL;
				return NULL;
			}
			fseek(BinFile, (w + 3) / 4 * 4 - w, SEEK_CUR);
		}
	}
	fclose(BinFile);
	return plmg;
}


bool Write8BitImg2BmpFile(BYTE *pImg, int width, int height, const char* filename)
//当宽度不是4的倍数时自动添加成4的倍数
{
	FILE *BinFile;
	BITMAPFILEHEADER FileHeader;
	BITMAPINFOHEADER BmpHeader;
	int i, extend;
	bool Suc = true;
	BYTE p[4], *pCur;

	//Open File
	if ((BinFile = fopen(filename, "w+b")) == NULL) { return false; }
	//Fill the FileHeader
	FileHeader.bfType = ((WORD)('M' << 8) | 'B');
	FileHeader.bfOffBits = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+256 * 4L;
	FileHeader.bfSize = FileHeader.bfOffBits + width*height;
	FileHeader.bfReserved1 = 0;
	FileHeader.bfReserved2 = 0;
	if (fwrite((void*)&FileHeader, 1, sizeof(FileHeader), BinFile) != sizeof(FileHeader)) Suc = false;
	//Fill the ImgHeader
	BmpHeader.biSize = 40;
	BmpHeader.biWidth = width;
	BmpHeader.biHeight = height;
	BmpHeader.biPlanes = 1;
	BmpHeader.biBitCount = 8;
	BmpHeader.biCompression = 0;
	BmpHeader.biSizeImage = 0;
	BmpHeader.biXPelsPerMeter = 0;
	BmpHeader.biYPelsPerMeter = 0;
	BmpHeader.biClrUsed = 0;
	BmpHeader.biClrImportant = 0;
	if (fwrite((void*)&BmpHeader, 1, sizeof(BmpHeader), BinFile) != sizeof(BmpHeader)) Suc = false;
	//write Pallete
	for (i = 0, p[3] = 0; i<256; i++)
	{
		p[3] = 0;
		p[0] = p[1] = p[2] = i;//blue,green,red
		if (fwrite((void*)p, 1, 4, BinFile) != 4) { Suc = false; break; }
	}
	//write image data
	extend = (width + 3) / 4 * 4 - width;
	if (extend == 0)
	{
		for (pCur = pImg + (height - 1)*width; pCur >= pImg; pCur -= width)
		{
			if (fwrite((void*)pCur, 1, width, BinFile) != (unsigned int)width) Suc = false;//真实的数据
		}
	}
	else
	{
		for (pCur = pImg + (height - 1)*width; pCur >= pImg; pCur -= width)
		{
			if (fwrite((void*)pCur, 1, width, BinFile) != (unsigned int)width) Suc = false;//真实的数据
			for (i = 0; i<extend; i++)//扩充的数据
			if (fwrite((void*)(pCur + width - 1), 1, 1, BinFile) != 1) Suc = false;
		}
	}
	//return
	fclose(BinFile);
	return Suc;
}

bool Template(BYTE *pGryImg, long width, long height, float * tempArray)
{
	long LineBytes;
	LineBytes = WIDTHBYTES(width * 8);
	//prepare for cuda
	long *LineBytesGPU;
	long *heightGPU;
	long *widthGPU;
	BYTE *NewpGryImgGPU;
	BYTE *pGryImgGPU;
	float *tempArrayGPU;
	cudaMalloc(&LineBytesGPU, sizeof(long));
	cudaMalloc(&heightGPU, sizeof(long));
	cudaMalloc(&widthGPU, sizeof(long));
	cudaMalloc(&tempArrayGPU, sizeof(float)*9);
	cudaMalloc(&NewpGryImgGPU, sizeof(BYTE)*(LineBytes*height));
	cudaMalloc(&pGryImgGPU, sizeof(BYTE)*(LineBytes*height));
	cudaMemcpy(LineBytesGPU, &LineBytes, sizeof(long), cudaMemcpyHostToDevice);
	cudaMemcpy(heightGPU, &height, sizeof(long), cudaMemcpyHostToDevice);
	cudaMemcpy(widthGPU, &width, sizeof(long), cudaMemcpyHostToDevice);
	cudaMemcpy(tempArrayGPU, tempArray, sizeof(float)*9, cudaMemcpyHostToDevice);
	cudaMemcpy(pGryImgGPU, pGryImg, sizeof(BYTE)*(LineBytes*height), cudaMemcpyHostToDevice);
	//kernel proce
	int blockPerGird = ((height - 3 + 1)*(width - 3 + 1)/512) + 1;
	update<<<blockPerGird,512>>>(NewpGryImgGPU, LineBytesGPU, heightGPU, widthGPU, pGryImgGPU, tempArrayGPU);
	cudaThreadSynchronize();
	//prepare for cpu op
	cudaMemcpy(pGryImg, NewpGryImgGPU, (LineBytes*height)*sizeof(BYTE), cudaMemcpyDeviceToHost);
	//memcpy(pGryImg, NewpGryImg, LineBytes*height);
	return TRUE;
}

bool Sobel(BYTE *pGryImg, int width, int height, int threshold)
{
	BYTE *lpDst1;
	BYTE *lpDst2;
	BYTE *NewpGryImg1;
	BYTE *hNewGryImg1;
	BYTE *NewpGryImg2;
	BYTE *hNewGryImg2;
	long i, j;
	float aTemplate[9];

	hNewGryImg1 = (BYTE*)malloc(sizeof(BYTE)*width * height);
	if (hNewGryImg1 == NULL)
	{
		return FALSE;
	}
	NewpGryImg1 = hNewGryImg1;
	hNewGryImg2 = (BYTE*)malloc(sizeof(BYTE)*width * height);
	if (hNewGryImg2 == NULL)
	{
		return FALSE;
	}
	NewpGryImg2 = hNewGryImg2;
	lpDst1 = NewpGryImg1;
	memcpy(NewpGryImg1, pGryImg, width * height);
	lpDst2 = NewpGryImg2;
	memcpy(NewpGryImg2, pGryImg, width * height);
	// 设置Sobel模板参数
	aTemplate[0] = -1.0;
	aTemplate[1] = -2.0;
	aTemplate[2] = -1.0;
	aTemplate[3] = 0.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 0.0;
	aTemplate[6] = 1.0;
	aTemplate[7] = 2.0;
	aTemplate[8] = 1.0;
	if (!Template(NewpGryImg1, width, height, aTemplate))
	{
		return FALSE;
	}
	// 设置Sobel模板参数
	aTemplate[0] = -1.0;
	aTemplate[1] = 0.0;
	aTemplate[2] = 1.0;
	aTemplate[3] = -2.0;
	aTemplate[4] = 0.0;
	aTemplate[5] = 2.0;
	aTemplate[6] = -1.0;
	aTemplate[7] = 0.0;
	aTemplate[8] = 1.0;
	if (!Template(NewpGryImg2, width, height, aTemplate))
	{
		return FALSE;
	}
	for (j = 0; j <height; j++)
	{
		for (i = 0; i <width - 1; i++)
		{
			lpDst1 = NewpGryImg1 + width * j + i;
			lpDst2 = NewpGryImg2 + width * j + i;
			if (*lpDst2 > *lpDst1)
			{
				*lpDst1 = *lpDst2;
			}
			if (*lpDst1>threshold)
			{
				*lpDst1 = 255;
			}
			if (*lpDst1<threshold)
			{
				*lpDst1 = 0;
			}
		}
	}
	memcpy(pGryImg, NewpGryImg1, width * height);
	return TRUE;
}




int main()
{
	clock_t start, finish;
	int width, height, threshold = 0;
	start = clock();
	BYTE *pGryImg = Read8BitBmpFile2Img("lena.bmp", &width, &height);
	BYTE *dstImg = (BYTE*) malloc(sizeof(BYTE)*width*height);
	//printf("%d,%d\n",width,height);  
	printf("Robert算子边缘检测\n");
	//printf("请输入阈值:");
	//scanf("%d", &threshold);
	threshold = 50;
	//pGryImg = Read8BitBmpFile2Img("lena.bmp", &width, &height);
	Sobel(pGryImg, width, height, threshold);
	Write8BitImg2BmpFile(pGryImg, width, height, "lena_Sobel.bmp");
	free( pGryImg);
	finish = clock();
	float time = 0;
	printf("time:%f\n", time = (float)(finish - start / CLOCKS_PER_SEC));

	return 0;
}
