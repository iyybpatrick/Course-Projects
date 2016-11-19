#include<stdlib.h>
#include<time.h>
#include<stdio.h>


struct pcb{      
    int status;     //process status
    int length;    //output length
}*PCB[3];

struct req{      //request process
    int reqname;  //request name
    int length;    //output length
    int addr;  //first address
}reqblock[10];

int buffer[2][100];
int head=0,tail=0;
int t1=5,t2=5;
FILE *f;

void help();
void menu();

//request function
void request(int i) 
{
    int j,m,s,length=0;
    struct req*run;
    if(1==i)
        t1--;
    else
        t2--;
    printf("user %d requests for data:\n",i);//输出标注
    fprintf(f,"user %d requests for data:\n",i);
    run=&reqblock[tail%10];//output block
    run->reqname=i;
    run->length=0;
    if(0==tail)
        run->addr=0;
    else
    {
        int index=(tail-1)%10;
        run->addr=reqblock[index].addr+reqblock[index].length;
    }
    for( m=0;m<100;m++)
    {
        if(0==buffer[i-1][m])
        {
            run->addr=m;
            break;
        }
    }
     s=0;
    while(1)
    {
        j=rand()%10;
        if(0==j)
        {
            run->length=length;
            break;
        }
        buffer[i-1][(run->addr+length)]=s;
        printf("%3d",s);
        fprintf(f,"%3d",s);
        s++;
        length++;
    }
    printf("\n");
    fprintf(f,"\n");
    PCB[i-1]->length+=length;
    length=0;
    if(2==PCB[2]->status)
        PCB[2]->status=0;
    tail++;
}

void spooling()
{
    int i,j;
    struct req*run;
    printf("Call SPOOLING to output data:\n");
    fprintf(f,"Call SPOOLING to output data:\n");
    run=&reqblock[head%10];
    printf("%d:",run->reqname);
    fprintf(f,"%d:",run->reqname);
    for(i=0;i<run->length;i++)
    {
        printf("%3d",buffer[run->reqname-1][run->addr+i] );
        fprintf(f,"%3d",buffer[run->reqname-1][run->addr+i] );
    }
    printf("\n");
    fprintf(f,"\n");
    head++;
    for( j=0;j<2;j++)
    {
        if(1==PCB[j]->status)
            PCB[j]->status=0;
    }
}

void start()
{ int l,m,n,j,k;
    for(l=0;l<2;l++)
        for(j=0;j<100;j++)
            buffer[l][j]=0;
    for(n=0;n<3;n++)
    {
        struct pcb*tmpPcb=(struct pcb*)malloc(sizeof(struct pcb));
        tmpPcb->status=0;
        tmpPcb->length=0;
        PCB[n]=tmpPcb;
    }
    printf("两个用户进程的请求分别为5，5.\n");
    fprintf(f,"两个用户进程的请求分别为5，5.\n");
    srand((unsigned)time(NULL));
    while(1)
    {

        k=rand()%100; //random number to simulate executive probability
        if(k<=45)//execute request output process
        {
            if((0==PCB[0]->status)&&(t1>0))
                request(1);
        }
        else if((k<=90)&&(t2>0))//execute request output process 2
        {
            if(0==PCB[1]->status)
                request(2);
        }
        else
            spooling();//process spooling
        if((0==t1)&&(0==t2)&&(head==tail))
            break;
    }
    for(m=0;m<3;m++)
    {
        free(PCB[m]);
        PCB[m]=NULL;//PCB empty.
    }
    printf("\Enter return to menu\n");
    getchar();
    tail=0;
    head=0;
    t1=5;
    t2=5;
    menu();
}
 
void help()
{
	
	printf("\n本程序为SPOOLING技术的模拟程序\n");
	printf("\nSPOOLING是Simultaneous Peripheral Operation On-Line \n");
	printf("的缩写，它是关于慢速字符设备如何与计算机主机交换信息的一种技术,\n");
	printf("通常称为“假脱机技术”。\n");
	printf("SPOOLING技术实际上是一种外围设备同时联机操作技术,\n");
	printf("又称为排队转储技术。\n");
	printf("它在输入和输出之间增加了“输入井”和“输出井”的排队转储环节.\n");
	printf("尝试通过C语言程序模拟SPOOLING技术\n");
	printf("本实验的结果保存在C盘，result文件中。\n");
    printf("\n按回车键返回菜单选项\n");
	getchar();
	menu();
}

void menu() 
{
	int k;  
	long i; 
	printf("**********************************************************\n\n"); 
	printf("\n                  SPOOLING 技术模拟实现               \n\n"); 	
	printf("\n|******          1.开始模拟SPOOLING技术         ******|\n"); 
	printf("\n|******          2.帮助文档                     ******|\n"); 
	printf("\n|******          3.退出程序                     ******|\n\n"); 
	printf("\n**********************************************************\n"); 
	printf("\n请输入选择号(1--3):"); 
	scanf("%d",&k); 
	getchar();
	switch(k) 
	{ 
		case 1:start();break; 
		case 2:help(); break; 
		case 3:exit(0); break; 
		default:break; 
	} 
}

main()
{
	f=fopen("C:\\result.txt","w");
	menu();
	fclose(f);
}
