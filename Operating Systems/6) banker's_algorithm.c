#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define FALSE 0
#define TRUE 1
#define M 20  //Max process
#define N 20  //Max resource type
int ProcNum;//Process number
int SourNum;//resource number
int id; //process ID

int Available[N];	//available resource
int MAX[M][N];		//Max matrix
int Need[M][N];		//Need matrix
int Allocation[M][N];	//delivery matrix
int Request[N];		//request matrix
int Work[N];		//work matrix
int Finish[M];          //done matrix
int work_allocation[M][N];

void InitSystem();
void PrintInfo();
int TryAllocate();
void Recall();
int CheckSafe();

int flag = FALSE;
int safe[M];


int main(void)
{
	int ret = FALSE;
	int option;
	while (1)
	{
		printf("******0.Init system\n");
		printf("******1.Ask for allocation\n");
		printf("******2.Show allocation table\n");
		printf("******3.Checking if safe\n");
		printf("******4.Exit\n");
		printf("please input:");
		scanf("%d", &option);
		if (option == 0)
			InitSystem();
		else if (option == 1)
		{
			if (TryAllocate() == TRUE)
			{
				flag = CheckSafe();
				if (flag == FALSE)
				{
					printf(" System unsafe, cannot allocate. \n");
					Recall();
				}
				else{
					printf(" System safe, allocate \n");
					PrintInfo();
				}
			}
			else{
				printf(" allocate fails! \n");
				Recall();
			}
		}
		else if (option == 2)
			PrintInfo();
		else if (option == 3)
		{
			if ((flag = CheckSafe()) == FALSE)
				printf("Unsafe now!\n");
			else	printf("safe now！\n");
		}
		else if (option == 4)
			break;
		else
			printf("Please input again \n");
	}
}

void InitSystem()
{
	int i = 0, j = 0;
	char ch;
	int temp;
	printf("Please Input the number of resource type:\n");
	scanf("%d", &SourNum);
	printf("Please input process number:\n");
	scanf("%d", &ProcNum);
	printf("Please input resource available number:\n");
	for (; i < SourNum; i++)
		scanf("%d", &Available[i]);
	printf("Please input maximum need for different resource:\n");
	for (i = 0; i <ProcNum; i++)
	{
		printf("Process P%d:\n", i);
		for (j = 0; j < SourNum; j++)
		{
			scanf("%d", &MAX[i][j]);
		}
	}
	ch = getchar();
	printf("Please input allocate matrix:");
	for (i = 0; i < ProcNum; i++)
	{
		printf("Process P%d:\n", i);
		for (j = 0; j < SourNum; j++)
		{
			scanf("%d", &Allocation[i][j]);
		}
	}
	bzero(Request, sizeof(Request));
	bzero(Work, sizeof(Work));
	bzero(Finish, sizeof(Finish));
	/*Initial Need*/
	for (i = 0; i<ProcNum; i++)
	for (j = 0; j<SourNum; j++)
	{
		Need[i][j] = MAX[i][j] - Allocation[i][j];
		work_allocation[i][j] = Allocation[i][j] + Work[j];
	}
}

void Recall()
{
	int i, j;
	bzero(Finish, sizeof(Finish));
	for (i = 0; i < SourNum; i++)
	{
		Available[i] += Request[i];
		Allocation[id][i] -= Request[i];
		Need[id][i] += Request[i];
	}
	bzero(work_allocation, sizeof(work_allocation));
	for (i = 0; i<ProcNum; i++)
	for (j = 0; j<SourNum; j++)
	{
		work_allocation[i][j] = Allocation[i][j];
	}
}


void PrintInfo()
{
	int i, j, k;
	printf("\t  Work             Need          Allocation    Work+Allocation Finish\n");
	for (j = 0; j < 4; j++)
	{
		printf("\t  ");
		for (i = 0; i < SourNum; i++)
			printf("%c  ", (char)('A' + i));
	}
	printf("\n");
	for (k = 0; k<ProcNum; k++)
	{
		if (flag == TRUE)
			i = safe[k];
		else i = k;
		printf("P%d\t", i);
		for (j = 0; j<SourNum; j++)
			printf("%3d", work_allocation[i][j] - Allocation[i][j]);
		printf("\t");
		for (j = 0; j<SourNum; j++)
			printf("%3d", Need[i][j]);
		printf("\t");
		for (j = 0; j<SourNum; j++)
			printf("%3d", Allocation[i][j]);
		printf("\t");
		for (j = 0; j<SourNum; j++)
			printf("%3d", work_allocation[i][j]);
		printf("\t");
		if (Finish[i] == FALSE)
			printf("false\n");
		else printf("true\n");
	}
}

/* Check whether it is safe now */
int CheckSafe()
{
	int i, j;
	int count = 0;
	bzero(work_allocation, sizeof(work_allocation));
	printf("**********Work Vector**********\n");
	for (i = 0; i < SourNum; i++)
	{
		Work[i] = Available[i];
		work_allocation[0][i] = Work[i];
	}
	for (i = 0; i < ProcNum; i++)
		Finish[i] = FALSE;
	for (i = 0; i < ProcNum; i++)
	{
		if (Finish[i] == FALSE)
		{
			for (j = 0; j<SourNum; j++)
			{
				if (Need[i][j] > Work[j])
					break;
			}
			if (j == SourNum)
			{
				printf("P%d\t", i);
				safe[count++] = i;
				for (j = 0; j<SourNum; j++)
				{
					Work[j] += Allocation[i][j];
					work_allocation[i][j] = Work[j];
					printf("%d\t", Work[j]);
				}
				printf("\n");
				Finish[i] = TRUE;  
				if (i == ProcNum - 1)
					i = -1;//Restart
			}
		}
	}
	printf("\n");
	for (i = 0; i<ProcNum; i++)
	{
		if (Finish[i] == FALSE)
		{
			printf("Process P%d cannot be finished！\n", i);
			return FALSE;
		}
	}
	/*如果前面未返回，则存在安全序列*/
	printf("feasible vector quality are listed as follows:\n");
	for (i = 0; i < ProcNum; i++)
		printf("P%d", safe[i]);
	printf("\n");
	return TRUE;
}


int TryAllocate()
{
	int i, temp, j;
	int RET = TRUE;
	for (i = 0; i<ProcNum; i++)
	for (j = 0; j<SourNum; j++)
		work_allocation[i][j] = Allocation[i][j] + Work[j];

	printf("Please input process ID:\n");
	while (scanf("%d", &id))
	{
		if (id < 0 | id > ProcNum)
			printf("Process not exist!\n");
		else break;
	}
	printf("Please input resource data:\n");
	for (i = 0; i < SourNum; i++)
	{
		scanf("%d", &Request[i]);
	}
	printf("Allocating data for process P%d:\n", id);
	for (i = 0; i < SourNum; i++)
	{
		if (Request[i] > Need[id][i])
		{
			printf("Request data exceed maximum resource number,fail.\n");
			RET = FALSE;
		}
		else if (Request[i] > Available[i]){
			printf("Request data exceed available resource number,fail.\n");
			RET = FALSE;
		}
		Available[i] -= Request[i];
		Allocation[id][i] += Request[i];
		Need[id][i] -= Request[i];
	}
	return RET;
}