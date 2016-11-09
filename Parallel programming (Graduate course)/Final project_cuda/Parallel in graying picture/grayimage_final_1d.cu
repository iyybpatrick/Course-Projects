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
int height; //图像高度
int biClrUsed; //实际使用的颜色表索引数
RGBQUAD *pColorTable; //颜色表指针
unsigned char *pBmpBuf; //图像数据指针
int biBitCount; //图像像素位数
int BLOCK_NUM;


int readBmp(char *bmpName){
	FILE *fp; //二进制打开文件
	BITMAPINFOHEADER head; //位图信息头
	int lineByte;
	
	fp=fopen(bmpName,"rb");
	//如果指针未指向任何图像
	if(fp==NULL){
		return 0;
	}
	//跳过位图文件文件头
	fseek(fp,sizeof(BITMAPFILEHEADER),0);	//将指针移动到距离开始大小为BITMAPFILEHEADER字节处
	printf("1..%ld\n",ftell(fp));

	fread(&head,sizeof(BITMAPINFOHEADER),1,fp);
	printf("size unsigned int%ld,\n",sizeof(unsigned int));
	printf("sizeof%ld,%ld\n",sizeof(BITMAPFILEHEADER),sizeof(BITMAPINFOHEADER));
	printf("%ld,%ld,%ld,%ld\n",sizeof(head.biWidth),sizeof(head.biHeight),sizeof(head.biBitCount),sizeof(head.biClrUsed));
	printf("2..%ld\n",ftell(fp));
	//通过head获取图像宽度，高度等信息
	width=head.biWidth;		//图像的宽
	height=head.biHeight;	//图像的高
	biBitCount=head.biBitCount;		//像素位数
	biClrUsed=head.biClrUsed;		//实际使用的颜色数
	//计算图像每行占用像素数
	lineByte=(width*biBitCount/8+3)/4*4;	//注意位图每行占据字节数必须是4的倍数（+3作用）
	
	//如果要读取的是灰度图像,需要读取颜色表
	if(biBitCount==8){
		pColorTable=(RGBQUAD*)malloc(sizeof( RGBQUAD)*256);
		fread(pColorTable,sizeof(RGBQUAD),256,fp); //此时fp并不是在文件最初位置
	}
	
	//读取位图图像数据
	pBmpBuf=(unsigned char*)malloc(sizeof(unsigned char)*(lineByte*height));
	pBmpBuf_size=lineByte*height;
	fread(pBmpBuf,1,lineByte*height,fp); //每次读一个字节
	fclose(fp); //关闭文件
	
	return 1;
}

/*******************************************************************
*	函数名：saveBmp()                                              *
*	功能：保存灰度图                                               *
*	函数参数：                                                     *
*	char *bmpName //保存灰度图名称-                                *
*	unsigned char *bmpBuf //位图数据指针，用来判断是否有数据传入   *
*	int pWidth //位图宽                                            *
*	int pHeight //位图高                                           *
*	int biBitCount //位图像素位数                                  *
*	RGBQUAD *pColorTable //颜色表指针                              *
*******************************************************************/
int saveBmp(char *bmpName,unsigned char *bmpBuf,int pWidth,int pHeight,int biBigCount,RGBQUAD *pColorTable){
	//如果无数据传入
	if(bmpBuf==NULL){
		return 0;
	}
	int colorTableSize=0;//定义颜色表大小
	//如果要保存的是灰度图
	if(biBitCount==8){
		colorTableSize=1024;//灰度图的颜色表大小为256*4=1024
	}
	int lineByte=(pWidth*biBitCount/8+3)/4*4;//每行占据的字节数
	//创建二进制文件
	FILE *fp=fopen(bmpName,"wb");
	//如果创建文件失败
	if(fp==NULL){
		printf("Creat file failed !");
		return 0;
	}
	//创建位图图像文件头
	BITMAPFILEHEADER filehead;//位图文件文件头
	filehead.bfType=0x4D42; //bmp 位图格式
	filehead.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+colorTableSize+lineByte*pHeight;
	filehead.bfReserved1=0;
	filehead.bfReserved2=0;
	//bfOffBits 的值是位图文件前三部分所需空间之和
	filehead.bfOffBits=54+colorTableSize;
	//将文件头写进文件中
	fwrite(&filehead,sizeof(BITMAPFILEHEADER),1,fp);
	
	//创建位图信息头
	BITMAPINFOHEADER infohead;
	//设置信息头相关参数
	infohead.biBitCount=biBitCount; //设置图像像素位数
	infohead.biClrImportant=0; //图像颜色索引都重要
	infohead.biClrUsed=0; //使用颜色表中的所有颜色索引
	infohead.biCompression=0; //图像压缩编码
	infohead.biHeight=pHeight; //图像高度
	infohead.biPlanes=1; //为目标设备说明位面数，其值将总是被设为1
	infohead.biSize=40; //BITMAPINFOHEADER结构所需要的字数
	infohead.biSizeImage=lineByte*pHeight; //图像大小
	infohead.biWidth=pWidth;
	infohead.biXPelsPerMeter=0; //水平分辨率
	infohead.biYPelsPerMeter=0; //竖直分辨率
	
	//将位图信息头写进文件
	fwrite(&infohead,sizeof(BITMAPINFOHEADER),1,fp);
	
	//如果是灰度图，要将灰度图的颜色表也写进文件才可以
	if(biBitCount==8){
		fwrite(pColorTable,sizeof(RGBQUAD),256,fp);
	}
	//将位图数据写入文件
	fwrite(bmpBuf,lineByte*pHeight,1,fp); //一次写一个字节
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
	char readPath[]="10000_7500.bmp"; //读入图像名称
	readBmp(readPath); //读入图像
	int gray; //灰度值
	
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
	//如果是256色图像
	if(biBitCount==8){
		for(i=0;i<256;i++){
			gray=(11*pColorTable[i].rgbBlue+30*pColorTable[i].rgbRed+59*pColorTable[i].rgbGreen)/100; //计算灰度值
			pColorTable[i].rgbBlue=(BYTE)gray; //将蓝色分量替换为灰度值
			pColorTable[i].rgbRed=(BYTE)gray; //将红色分量替换为灰度值
			pColorTable[i].rgbGreen=(BYTE)gray; //将绿色分量替换为灰度值
		}
	}
	
	//保存灰度图像
	char writePath[]="10000_7500cu.bmp";
	saveBmp(writePath,pBmpBuf,width,height,biBitCount,pColorTable);//存储灰度图像
	//清除new语句生成的全局变量
	free(pBmpBuf);
	//如果是灰度图
	if(biBitCount==8)
		free(pColorTable);
	gettimeofday(&t2,NULL);
	timeuse=t2.tv_sec-t1.tv_sec+(t2.tv_usec-t1.tv_usec)/1000000.0;
	printf("\ntime:%f\n",timeuse);
	return 0;	
}
	
	
