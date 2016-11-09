#include<sys/time.h>
#include<stdio.h>
#include<stdlib.h>
#include"bmp.h"
#include"function.h"
#include <cuda_runtime.h>
#include<malloc.h>
#define THREAD_NUM 512
int pBmpBuf_size;
int width; 
int height; //ͼ��߶�
int biClrUsed; //ʵ��ʹ�õ���ɫ��������
RGBQUAD *pColorTable; //��ɫ��ָ��
unsigned char *pBmpBuf; //ͼ������ָ��
int biBitCount; //ͼ������λ��
int BLOCK_NUM;


int readBmp(char *bmpName){
	FILE *fp; //�����ƴ��ļ�
	BITMAPINFOHEADER head; //λͼ��Ϣͷ
	int lineByte;
	
	fp=fopen(bmpName,"rb");
	//���ָ��δָ���κ�ͼ��
	if(fp==NULL){
		return 0;
	}
	//����λͼ�ļ��ļ�ͷ
	fseek(fp,sizeof(BITMAPFILEHEADER),0);	//��ָ���ƶ������뿪ʼ��СΪBITMAPFILEHEADER�ֽڴ�
	printf("1..%ld\n",ftell(fp));

	fread(&head,sizeof(BITMAPINFOHEADER),1,fp);
	printf("size unsigned int%ld,\n",sizeof(unsigned int));
	printf("sizeof%ld,%ld\n",sizeof(BITMAPFILEHEADER),sizeof(BITMAPINFOHEADER));
	printf("%ld,%ld,%ld,%ld\n",sizeof(head.biWidth),sizeof(head.biHeight),sizeof(head.biBitCount),sizeof(head.biClrUsed));
	printf("2..%ld\n",ftell(fp));
	//ͨ��head��ȡͼ���ȣ��߶ȵ���Ϣ
	width=head.biWidth;		//ͼ��Ŀ�
	height=head.biHeight;	//ͼ��ĸ�
	biBitCount=head.biBitCount;		//����λ��
	biClrUsed=head.biClrUsed;		//ʵ��ʹ�õ���ɫ��
	//����ͼ��ÿ��ռ��������
	lineByte=(width*biBitCount/8+3)/4*4;	//ע��λͼÿ��ռ���ֽ���������4�ı�����+3���ã�
	
	//���Ҫ��ȡ���ǻҶ�ͼ��,��Ҫ��ȡ��ɫ��
	if(biBitCount==8){
		pColorTable=(RGBQUAD*)malloc(sizeof( RGBQUAD)*256);
		fread(pColorTable,sizeof(RGBQUAD),256,fp); //��ʱfp���������ļ����λ��
	}
	
	//��ȡλͼͼ������
	pBmpBuf=(unsigned char*)malloc(sizeof(unsigned char)*(lineByte*height));
	pBmpBuf_size=lineByte*height;
	fread(pBmpBuf,1,lineByte*height,fp); //ÿ�ζ�һ���ֽ�
	fclose(fp); //�ر��ļ�
	
	return 1;
}

/*******************************************************************
*	��������saveBmp()                                              *
*	���ܣ�����Ҷ�ͼ                                               *
*	����������                                                     *
*	char *bmpName //����Ҷ�ͼ����-                                *
*	unsigned char *bmpBuf //λͼ����ָ�룬�����ж��Ƿ������ݴ���   *
*	int pWidth //λͼ��                                            *
*	int pHeight //λͼ��                                           *
*	int biBitCount //λͼ����λ��                                  *
*	RGBQUAD *pColorTable //��ɫ��ָ��                              *
*******************************************************************/
int saveBmp(char *bmpName,unsigned char *bmpBuf,int pWidth,int pHeight,int biBigCount,RGBQUAD *pColorTable){
	//��������ݴ���
	if(bmpBuf==NULL){
		return 0;
	}
	int colorTableSize=0;//������ɫ���С
	//���Ҫ������ǻҶ�ͼ
	if(biBitCount==8){
		colorTableSize=1024;//�Ҷ�ͼ����ɫ���СΪ256*4=1024
	}
	int lineByte=(pWidth*biBitCount/8+3)/4*4;//ÿ��ռ�ݵ��ֽ���
	//�����������ļ�
	FILE *fp=fopen(bmpName,"wb");
	//��������ļ�ʧ��
	if(fp==NULL){
		printf("Creat file failed !");
		return 0;
	}
	//����λͼͼ���ļ�ͷ
	BITMAPFILEHEADER filehead;//λͼ�ļ��ļ�ͷ
	filehead.bfType=0x4D42; //bmp λͼ��ʽ
	filehead.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+colorTableSize+lineByte*pHeight;
	filehead.bfReserved1=0;
	filehead.bfReserved2=0;
	//bfOffBits ��ֵ��λͼ�ļ�ǰ����������ռ�֮��
	filehead.bfOffBits=54+colorTableSize;
	//���ļ�ͷд���ļ���
	fwrite(&filehead,sizeof(BITMAPFILEHEADER),1,fp);
	
	//����λͼ��Ϣͷ
	BITMAPINFOHEADER infohead;
	//������Ϣͷ��ز���
	infohead.biBitCount=biBitCount; //����ͼ������λ��
	infohead.biClrImportant=0; //ͼ����ɫ��������Ҫ
	infohead.biClrUsed=0; //ʹ����ɫ���е�������ɫ����
	infohead.biCompression=0; //ͼ��ѹ������
	infohead.biHeight=pHeight; //ͼ��߶�
	infohead.biPlanes=1; //ΪĿ���豸˵��λ��������ֵ�����Ǳ���Ϊ1
	infohead.biSize=40; //BITMAPINFOHEADER�ṹ����Ҫ������
	infohead.biSizeImage=lineByte*pHeight; //ͼ���С
	infohead.biWidth=pWidth;
	infohead.biXPelsPerMeter=0; //ˮƽ�ֱ���
	infohead.biYPelsPerMeter=0; //��ֱ�ֱ���
	
	//��λͼ��Ϣͷд���ļ�
	fwrite(&infohead,sizeof(BITMAPINFOHEADER),1,fp);
	
	//����ǻҶ�ͼ��Ҫ���Ҷ�ͼ����ɫ��Ҳд���ļ��ſ���
	if(biBitCount==8){
		fwrite(pColorTable,sizeof(RGBQUAD),256,fp);
	}
	//��λͼ����д���ļ�
	fwrite(bmpBuf,lineByte*pHeight,1,fp); //һ��дһ���ֽ�
	fclose(fp);
	
	return 1;
}


__global__ void gray_f(unsigned char *pBmpBuf,int *lineByte,int *height,int *width)
{
	int gray;
	int i,j;
	BYTE red,green,blue;
	int tid=threadIdx.x;
	int bid=blockIdx.x;
	i=bid*THREAD_NUM+tid;
	if(i<*height)
	{
		for(j=0;j<*width;j++)
		{
			blue=*(pBmpBuf+i*(*lineByte)+j*3+0);
			green=*(pBmpBuf+i*(*lineByte)+j*3+1);
			red=*(pBmpBuf+i*(*lineByte)+j*3+2);
			gray=(30*red+59*green+11*blue)/100;
			*(pBmpBuf+i*(*lineByte)+j*3+0)=(BYTE)gray; 
			*(pBmpBuf+i*(*lineByte)+j*3+1)=(BYTE)gray; 
			*(pBmpBuf+i*(*lineByte)+j*3+2)=(BYTE)gray; 
		}
	}
}

int main(){
	struct timeval t1,t2;
	double timeuse;
	gettimeofday(&t1,NULL);
	char readPath[]="10000_7500.bmp"; //����ͼ������
	readBmp(readPath); //����ͼ��
	int gray; //�Ҷ�ֵ
	
	printf("the width: %d\nthe height: %d\nthe biBitCount: %d\n",width,height,biBitCount);
	printf("the biclrused: %d\n",biClrUsed);
	int i;
	int lineByte=(width*biBitCount/8+3)/4*4;
	int *a;
	unsigned char *b;
	int *c,*d;
	cudaMalloc(&a,sizeof(int));
	cudaMalloc(&c,sizeof(int));
	cudaMalloc(&d,sizeof(int));
	cudaMalloc(&b,sizeof(unsigned char)*pBmpBuf_size);
	cudaMemcpy(a,&lineByte,sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(c,&height,sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(d,&width,sizeof(int),cudaMemcpyHostToDevice);
	cudaMemcpy(b,pBmpBuf,sizeof(unsigned char)*pBmpBuf_size,cudaMemcpyHostToDevice);
	if(biBitCount==8){}
	else if(biBitCount==24){
				BLOCK_NUM=width/THREAD_NUM+1;
				gray_f<<<BLOCK_NUM,THREAD_NUM>>>(b,a,c,d);
				cudaMemcpy(pBmpBuf,b,sizeof(unsigned char)*pBmpBuf_size,cudaMemcpyDeviceToHost);
			}
		
	cudaFree(a);
	cudaFree(b);	
	cudaFree(c);	
	cudaFree(d);	
	//�����256ɫͼ��
	if(biBitCount==8){
		for(i=0;i<256;i++){
			gray=(11*pColorTable[i].rgbBlue+30*pColorTable[i].rgbRed+59*pColorTable[i].rgbGreen)/100; //����Ҷ�ֵ
			pColorTable[i].rgbBlue=(BYTE)gray; //����ɫ�����滻Ϊ�Ҷ�ֵ
			pColorTable[i].rgbRed=(BYTE)gray; //����ɫ�����滻Ϊ�Ҷ�ֵ
			pColorTable[i].rgbGreen=(BYTE)gray; //����ɫ�����滻Ϊ�Ҷ�ֵ
		}
	}
	
	//����Ҷ�ͼ��
	char writePath[]="10000_7500cu.bmp";
	saveBmp(writePath,pBmpBuf,width,height,biBitCount,pColorTable);//�洢�Ҷ�ͼ��
	//���new������ɵ�ȫ�ֱ���
	free(pBmpBuf);
	//����ǻҶ�ͼ
	if(biBitCount==8)
		free(pColorTable);
	gettimeofday(&t2,NULL);
	timeuse=t2.tv_sec-t1.tv_sec+(t2.tv_usec-t1.tv_usec)/1000000.0;
	printf("\ntime:%f\n",timeuse);
	return 0;	
}
	
	
