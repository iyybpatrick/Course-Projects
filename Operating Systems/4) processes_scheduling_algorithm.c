#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<windows.h>

#define GETJCB(type) (type*)malloc(sizeof(type))         //Allocate space for job
#define GETINFOR(type) (type*)malloc(sizeof(type))       //Allocate space for job's running time
int RESOURCES=101;                                       //default resource number
int NUM=0;                                               //default working number                                 
int TIME_HAND=0;                                         //default handling time
char *algorithm_name;                                    //saving algorithm name

//process information
typedef struct information{     
	int arrived_time;                                      //arriving time
	int served_time;                                       //serving time, (running time.)
	int begin_time;                                        //work starting time.
	int finished_time;                                     //work finishing time
	int turnaround_time;                                   //turnaround time=finishing time-starting time
	float qtt;                                             //qqt=turnaround time/running time
	float response_ratio;                                  //response ratio
}INFOR;
//Job information
struct jcb{                                             
	char j_name[3];                                        //Job name
	int j_hand;                                            //Job handling time
	int j_need_time;                                       //Job total time
	int j_resources;                                       //Required resource number , default
	char j_state;                                          //job's status
	struct jcb *j_pnext;                                   //next job's address
	INFOR *j_infor;
}*ready=NULL,*pjcb;
typedef struct jcb JCB;

/*把每种算法需要的顺序分别存储在不同地方，并且用指针指向开始位置*/
JCB *addr_fcfs;                                          //pointer pointing to the first address of FCFS algorithm
JCB *addr_sjf;                                           //pointer pointing to the first address of SJF algorithm
JCB *addr_hrn;                                           //pointer pointing to the first address of HRN algorithm

/*FCFS*/
void Sort_fcfs(JCB *fcfs_head)
{
	JCB *p;
	JCB *q;
	
	p=fcfs_head;
	q=p;
	p=p->j_pnext;
	addr_fcfs=q;
	
	while(p!=NULL)
	{
		q->j_pnext=p;
		q=q->j_pnext;
		p=p->j_pnext;
	}
}

/*For judging whether this job is in memory or not.*/
int Exist(JCB *p,JCB *q)
{
	int j=0;
	while(p!=NULL)
	{
		if(p->j_infor->arrived_time==q->j_infor->arrived_time)  //Judge by comparing it's arriving time with current time.
			return 1;
		else
			p=p->j_pnext;
			
	}

	return 0;
}

/*For filling working information for every algorithm*/
void Copy(JCB *s,JCB *p,int k)
{
	int i=0;
	for(;i<3;i++)                       
	s->j_name[i]=p->j_name[i];
	s->j_hand=p->j_hand;
	s->j_need_time=p->j_need_time;
	s->j_resources=p->j_resources;
	if(k==0)                            //Change the first algorithm's status into ready 'R', others to wait 'W'
		s->j_state='R';
	else
		s->j_state='W';                
	s->j_pnext=NULL;
	s->j_infor->arrived_time=p->j_infor->arrived_time;
	s->j_infor->served_time=p->j_infor->served_time;
	s->j_infor->begin_time=p->j_infor->begin_time;
	s->j_infor->finished_time=p->j_infor->finished_time;
	s->j_infor->response_ratio=p->j_infor->response_ratio;
	s->j_infor->turnaround_time=p->j_infor->turnaround_time;
	s->j_infor->qtt=p->j_infor->qtt;
}

/*For sorting SJF algorithm, and load working information to memory*/
void Sort_sjf(JCB *sjf_head)
{
	JCB *p;
	JCB *q;
	JCB *s;
	int i=0;
	int k=0;
	 
	p=sjf_head;                          //sjf_head address
	q=p->j_pnext; 
	
	while(q!=NULL)                       //init , find the shortest running time job's address
	{
		if(p->j_need_time > q->j_need_time)
			{
				p=q;
				q=p->j_pnext;
			}
		else
			q=q->j_pnext;
	}
	
	s=GETJCB(JCB);                       //init ,fill this shortest time job into memory
	s->j_infor=GETINFOR(INFOR);          
	Copy(s,p,k);                         //Copy information
	addr_sjf=s;                   
	
	for(;i<NUM-1;i++)                    //sort to find out other jobs with the shortest running time
	{
		p=sjf_head;                      
		for(;p!=NULL;)
		{
			if(Exist(addr_sjf,p)==1)        
				p=p->j_pnext;
			else
				break;
		}
		q=p->j_pnext;
		while(q!=NULL)
		{
			if(p->j_need_time > q->j_need_time)   //compare to find the job with shortest time
			{
				if(Exist(addr_sjf,q))
				{
					q=q->j_pnext;
				}
				else
				{
					p=q;
					q=p->j_pnext;
				}
			}
			else
			{
				q=q->j_pnext;
			}
		}		
		
		k++;
		s->j_pnext=GETJCB(JCB);                  // load the shortest time job into memory
		s->j_pnext->j_infor=GETINFOR(INFOR);
		Copy(s->j_pnext,p,k);
		s=s->j_pnext;	
	}
}

/* Sort HRN*/
void Sort_hrn(JCB *hrn_head)
{
	JCB *p;
	JCB *q;
	JCB *s;
	int i=0;
	int k=0;
	float sum_served_time=0.00;              //To save time remaining
	
	p=hrn_head;                           
	
	s=GETJCB(JCB);
	s->j_infor=GETINFOR(INFOR);
	p->j_infor->response_ratio=1.00;         //first job's response ratio=1.00
	sum_served_time=(float)p->j_need_time;   //total working time.
	if(p!=NULL)
	  Copy(s,p,k);                           //load to memory
	else
		{
			printf("No jobs.\n");
			exit(1);
		}
	addr_hrn=s;                              //addr_hrn points to the first address of memory
	p=p->j_pnext;
	
	for(;i<NUM-1;i++)
	{
		p=hrn_head;
		for(;p!=NULL;)
		{
			if(Exist(addr_hrn,p)==1)
				p=p->j_pnext;
			else
				break;
		}
		q=p->j_pnext;
		
		p->j_infor->response_ratio=((float)p->j_need_time+sum_served_time-(float)p->j_infor->arrived_time)/(float)p->j_need_time;
		while(q!=NULL)
		{
			q->j_infor->response_ratio=((float)q->j_need_time+sum_served_time-(float)q->j_infor->arrived_time)/(float)q->j_need_time;
			if(p->j_infor->response_ratio < q->j_infor->response_ratio)
			{
				if(Exist(addr_hrn,q)==1)
				{
					q=q->j_pnext;
				}
				else
				{
					p=q;
					p->j_infor->response_ratio = q->j_infor->response_ratio;
					q=p->j_pnext;
				}
			}
			else
			{
				q=q->j_pnext;
			}
		}
		
		k++;
		sum_served_time+=p->j_need_time;
		s->j_pnext=GETJCB(JCB);
		s->j_pnext->j_infor=GETINFOR(INFOR);
		Copy(s->j_pnext,p,k);
		s=s->j_pnext;
	}
}

/*init*/
void Initial()
{
	int i=0;
	JCB *p;
	printf("Input the testing job number :");
	scanf("%d",&NUM);

	pjcb=GETJCB(JCB);
	pjcb->j_infor=GETINFOR(INFOR);
	
	printf("Input testing job name(first 3 character) :");
	scanf("%s",&pjcb->j_name);
	printf("Input serving time:");
	scanf("%d",&pjcb->j_need_time);
	
	pjcb->j_resources=RESOURCES;
	pjcb->j_state='R';
	pjcb->j_hand=TIME_HAND;
	pjcb->j_infor->arrived_time=0;         //working arrival time
	pjcb->j_infor->response_ratio=0.00;    //response ratio default 0.00
	ready=pjcb;
	pjcb->j_pnext=NULL;
		
  if(ready!=NULL)
	for(;i<NUM-1;i++)
	{
		p=GETJCB(JCB);
		p->j_infor=GETINFOR(INFOR);
		printf("Input testing job name(first 3 character) :");
		scanf("%s",&p->j_name);
		printf("Input serving time:");
		scanf("%d",&p->j_need_time);
		
		p->j_resources=RESOURCES;
		p->j_state='W';
		p->j_hand=TIME_HAND;
		p->j_infor->arrived_time=i+1;
		p->j_infor->response_ratio=0.00;
		p->j_pnext=NULL;
		
		pjcb->j_pnext=p;
		pjcb=pjcb->j_pnext;
	}
	
	Sort_fcfs(ready);            //给每种算法进行排好序
	Sort_sjf(ready);
	Sort_hrn(ready);
}

/*choose*/
int Choice()
{
	int choice;
	int i=0;

	printf("\nPlease choose algorithm :\n");
	printf("1.FCFS   2.SJF   3.HRN \n");
	scanf("%d",&choice);
	if(choice==1)
		algorithm_name="FCFS";
	else if(choice==2)
		algorithm_name="SJF";
	else if(choice==3)
		algorithm_name="HRN";
	else
		{
			printf("Wrong enter, please try again!\n");
			exit(1);
		}

	return choice;
}

/*display*/
void Display_Queue(JCB *head)
{
	JCB *p;
	
	p=head;
	if(p!=NULL)
	{
	   printf("\nCurrent informtion :\n");
	   printf("+——————————————————————————————————————+\n");
	   printf("|—作业名—所需运行时间—所需资源—作业状态—下一个作业名————|\n");
	
	   while(p!=NULL)
	   {
	     printf("|—%s———%d—————%d———%c———%s————|\n",p->j_name,p->j_need_time,p->j_resources,p->j_state,p->j_pnext->j_name);
	  
	     p=p->j_pnext;
	     }
	    printf("+——————————————————————————————————————+\n");
	}
	else
		{
			printf("No job in queue！\n");
			exit(1);
		}
}

/*Display job state*/
void Display_State(JCB *jcb_state)
{
	JCB *p;
	int sum_turnaround_time=0;
	float sum_qtt=0.0;
	float aturnaround_time=0.0;
	float aqtt=0.0;
	
	p=jcb_state;
	
	printf("%s algorithm performance：\n",algorithm_name);
	printf("+——————————————————————————————————————————+\n");
	printf("|—作业名—到达时刻—服务时间—开始执行时刻—完成时刻—周转时间—带权周转时间——|\n");
	while(p!=NULL)
	{
	  sum_turnaround_time+=p->j_infor->turnaround_time;
		sum_qtt+=p->j_infor->qtt;
		
		printf("|—%s——%d——%d———%d———%d———%d———%.2f——|\n",p->j_name,p->j_infor->arrived_time,p->j_infor->served_time,p->j_infor->begin_time,p->j_infor->finished_time,p->j_infor->turnaround_time,p->j_infor->qtt);
		
		p=p->j_pnext;
	}
	printf("+——————————————————————————————————————————+\n");
	
	aturnaround_time=(float)sum_turnaround_time/(float)NUM;
	aqtt=sum_qtt/NUM;
	
	printf("\nworking information :turn around time :%.2f\n",aturnaround_time);
	printf("             :qqt :%.2f\n",aqtt);
}

/*diaplay HRN*/
void Display_state_hrn(JCB *jcb_state)
{
	JCB *p;
	int sum_turnaround_time=0;
	float sum_qtt=0.0;
	float aturnaround_time=0.0;
	float aqtt=0.0;
	
	p=jcb_state;
	
	printf("%s algorithm performance：\n",algorithm_name);
	printf("+——————————————————————————————————————————+\n");
	printf("|—作业名—到达时刻—服务时间—开始执行时刻—完成时刻—响应比—周转时间—带权周转时间——|\n");
	while(p!=NULL)
	{
	  sum_turnaround_time+=p->j_infor->turnaround_time;
		sum_qtt+=p->j_infor->qtt;
		
		printf("|—%s——%d——%d———%d———%d———%.2f———%d———%.2f——|\n",p->j_name,p->j_infor->arrived_time,p->j_infor->served_time,p->j_infor->begin_time,p->j_infor->finished_time,p->j_infor->response_ratio,p->j_infor->turnaround_time,p->j_infor->qtt);
		
		p=p->j_pnext;
	}
	printf("+——————————————————————————————————————————+\n");
	
	aturnaround_time=(float)sum_turnaround_time/(float)NUM;
	aqtt=sum_qtt/NUM;
	
	printf("\nworking information :turn around time :%.2f\n", aturnaround_time);
	printf("             :qqt :%.2f\n", aqtt);
}

/*显示运行的步骤*/
void Display_Running(JCB *run,int choice)
{
	JCB *p;
	JCB *pre_jcb;
	int i=0;
	
	p=run;
	pre_jcb=NULL;
	
	printf("running... ...\n");
	while(p!=NULL)
	{
		for(;i<p->j_need_time;i++)
		{
			printf("Current job name :%s   time remaining :%d\n",p->j_name,p->j_need_time-i);
			Sleep(1000);
		}
		if(p->j_pnext==NULL)
			{
				printf("Current job :%s has done.All have done.\n",p->j_name);
			}
		else
			printf("Current job :%s has done.,next job is :%s,please wait... ...\n",p->j_name,p->j_pnext->j_name);
		Sleep(2000);                        //delay to show
		printf("\n");
		
		p->j_infor->served_time=p->j_need_time;         
		if(pre_jcb==NULL)
		{
			p->j_infor->begin_time=0;
			p->j_infor->finished_time=p->j_need_time;
		}
		else
		{
			p->j_infor->begin_time=pre_jcb->j_infor->finished_time;     
		  p->j_infor->finished_time=p->j_infor->begin_time+p->j_infor->served_time;  
		}
		p->j_infor->turnaround_time=p->j_infor->finished_time-p->j_infor->arrived_time;   
		if(p->j_infor->turnaround_time < 0)
			p->j_infor->turnaround_time=-p->j_infor->turnaround_time;
		p->j_infor->qtt=(float)p->j_infor->turnaround_time/(float)p->j_infor->served_time; 
		
		i=0;
		pre_jcb=p;		
		p=p->j_pnext;
	}
	
	if(choice==3)                             
		Display_state_hrn(run);
	else
	    Display_State(run);
	
}

/*test FCFS algorithm*/
void FCFS_test()
{
	char answer;
	JCB *p;

  p=addr_fcfs;	                             //pass FCFS's first address to p
	
	Display_Queue(p);		                       
	printf("开始运行？(Y or N) :");
	getchar();
	scanf("%c",&answer);
	if(answer=='Y' || answer=='y')
		{
			Display_Running(p,1);                  //start running
		}
		else
			{
				printf("exiting... ...\n");
				Sleep(3000);
				exit(1);
			}
}

/*test SJF algorithm*/
void SJF_test()
{
	char answer;
	JCB *p;

  p=addr_sjf;	                                //pass SJF's first address to p
	
	Display_Queue(addr_sjf);		
	printf("start running ?(Y or N) :");
	getchar();
	scanf("%c",&answer);
	if(answer=='Y' || answer=='y')
		{
			Display_Running(p,2);
		}
		else
			{
				printf("exiting... ...\n");
				Sleep(3000);
				exit(1);
			}
}

/*test HRN algorithm*/
void HRN_test()
{
	char answer;
	JCB *p;

    p=addr_hrn;	                             //pass HRN's first address to p
	
	Display_Queue(addr_hrn);		
	printf("start running ?(Y or N) :");
	getchar();
	scanf("%c",&answer);
	if(answer=='Y' || answer=='y')
		{
			Display_Running(p,3);
		}
		else
			{
				printf("exiting... ...\n");	
				Sleep(3000);
				exit(1);
			}
}

/*运行的时候选择要测试的算法进行运行*/
void Running(int choice)
{
	char answer;

	switch(choice)
	{
	case 1:
		FCFS_test();
		break;
	case 2:
		SJF_test();
		break;
	case 3:
		HRN_test();
		break;
	default:
		break;
	}

	printf("continue ?(Y or N) ：");
	scanf("%s",&answer);
	if(answer=='Y' || answer=='y')
	{
		choice=Choice();
		Running(choice);
	}
	else
	{
		printf("exiting... ...\n");
		Sleep(3000);
		exit(1);
	}
}

/*release memory*/
void Destroy()
{
	int i=0;
	JCB *p1,*p2=NULL;
	JCB *q1,*q2=NULL;
	JCB *s1,*s2=NULL;
	
	p1=addr_fcfs;
	q1=addr_sjf;
	s1=addr_hrn;
	p2=p1->j_pnext;
	q2=q2->j_pnext;
	s2=s2->j_pnext;
	
	for(;i<NUM;i++)
	{
		free(p1);
		free(q1);
		free(s1);
		
		p1=p2;
		q1=q2;
		s1=s2;
		
		p2=p2->j_pnext;
		q2=q2->j_pnext;
		s2=s2->j_pnext;
	}
}

/*main*/
int main()
{
	int choice;         

	Initial();          
	choice=Choice();   
	Running(choice);    
	Destroy();         

	return 0;
}
