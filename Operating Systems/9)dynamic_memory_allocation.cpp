#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <malloc.h>
#include <windows.h>

struct freetable
{
	char id[10];
	int addr;
	int size;
	int flag;   //"0" means unoccupied£¬"1" means occupied
	struct freetable *next;
};
struct freetable *head;
void first(struct freetable *b);
void circle(struct freetable *c);
void best(struct freetable *d);
void callback(struct freetable *e);
void showmem(struct freetable *a);
void initial();

int main()
{
	int item;
	initial();

	while (1)
	{
		printf("................................................................................\n");
		printf("\t\t\t\tdynamic memory allocate:\n");
		printf("................................................................................\n");
		printf("\t\t\t1.First fit\n\t\t\t2.Circulation First fit\n\t\t\t3.Best fit\n\t\t\t4.Recycle memory\n\t\t\t5.Show memory\n\t\t\t0.ÍË³ö");
		printf("\n\t\t---->Please input your choice:");
		scanf("%d", &item);
		getchar();
		switch (item)
		{
		case 1:
			system("cls");
			printf("\t\t\t1.First fit\n");
			first(head);
			break;
		case 2:
			system("cls");
			printf("\t\t\t2.Circulation First fit\n");
			circle(head);
			break;
		case 3:
			system("cls");
			printf("\t\t\t3.Best fit\n");
			best(head);
			break;
		case 4:
			system("cls");
			printf("\t\t\t4.Recycle memory\n");
			callback(head);
			break;
		case 5:
			showmem(head);  //show memory
			break;
		case 0:
			exit(0);	//exit
		default:
			printf("\tWrong choice!\n");
			break;
		}
	}

	return 0;
}

void first(struct freetable *b)
{    //
	struct freetable *head, *i, *j, *l;
	char numstr[10];
	int k = 0;
	int temp;
	head = b;
	j = b;

	i = (struct freetable *)malloc(sizeof(struct freetable));
	printf("\n\nPlease input process name:");
	gets(i->id);
	printf("\nPlease input needed size:");
	gets(numstr);
	i->size = atoi(numstr);

	while (head != NULL)
	{
		if (head->flag == 0 && head->size >= i->size)
		{
			j = head;
			k = 1;
			break;
		}
		head = head->next;
	}
	if (k == 0)
	{
		printf("\nMemory lack enough space. Please try later.\n");
		return;
	}
	if (j->size == i->size)
	{
		strcpy(j->id, i->id);
		j->flag = 1;
	}
	else
	{
		l = (struct freetable *)malloc(sizeof(struct freetable));
		temp = j->size;
		j->size = i->size;
		strcpy(j->id, i->id);
		l->addr = j->addr + i->size;
		l->size = temp - i->size;

		if (l->size != 0)
		{
			l->next = j->next;
			l->flag = 0;
			strcpy(l->id, "¿ÕÏÐÇø");
			j->flag = 1;
			j->next = l;
		}
	}
	return;
}
//circulation first fit
void circle(struct freetable *c)
{
	struct freetable *head, *i, *j, *l;
	int k = 0;
	int temp;
	int njob, itemp;
	head = c;

	printf("\nPlease input process name:");
	scanf("%d", &njob);
	for (itemp = 0; itemp<njob; itemp++)
	{
		i = (struct freetable *)malloc(sizeof(struct freetable));
		printf("\n\nPlease input process name:");
		scanf("%s", i->id);
		printf("\nPlease input needed memory space:");
		scanf("%d", &i->size);

		while (head != NULL)
		{
			if (head->flag == 0 && head->size >= i->size)
			{
				j = head;
				k = 1;
				break;
			}
			head = head->next;
		}

		if (k == 0)
		{
			printf("\nMemory lack enough space,please try later!\n");
			return;
		}
		if (j->size == i->size)
		{
			strcpy(j->id, i->id);
			j->flag = 1;
		}
		else
		{
			l = (struct freetable *)malloc(sizeof(struct freetable));
			temp = j->size;
			j->size = i->size;
			strcpy(j->id, i->id);
			l->addr = j->addr + i->size;
			l->size = temp - i->size;

			if (l->size != 0)
			{
				l->next = j->next;
				l->flag = 0;
				strcpy(l->id, "¿ÕÏÐÇø");
				j->flag = 1;
				j->next = l;
			}
		}
		head = l;
	}
	return;
}
//Best fit
void best(struct freetable *d)
{
	struct freetable *head, *i, *j, *l;
	int sizetemp[9];
	int temp1, temp2;
	head = d;
	int k2 = 0;

	while (head != NULL)
	{
		if (head->flag == 0)
		{
			sizetemp[k2] = head->size;
			k2++;
		}

		head = head->next;
	}

	int temp3 = k2;

	for (int k1 = 0; k1<temp3 - 1; k1++)
	{
		int jtemp;
		int ok = 1;

		for (jtemp = 0; jtemp<k2 - k1; jtemp++)//sort
		{
			if (sizetemp[jtemp]>sizetemp[jtemp + 1])
			{
				ok = 0;
				temp1 = sizetemp[jtemp];
				sizetemp[jtemp] = sizetemp[jtemp + 1];
				sizetemp[jtemp + 1] = temp1;
			}
		}

		if (ok == 1)
			break;
	}

	int k = 0;
	int k4;
	head = d;
	i = (struct freetable *)malloc(sizeof(struct freetable));
	printf("\n\nPlease input process name:");
	scanf("%s", i->id);
	printf("\nplease input needed memory space:");
	scanf("%d", &i->size);

	for (int k3 = 0; k3<temp3; k3++)
	{
		if (sizetemp[k3] >= i->size)
		{
			k4 = k3;
			break;
		}
	}

	while (head != NULL)
	{
		if (head->flag == 0 && head->size == sizetemp[k4])
		{
			j = head;
			if (j->size == i->size)
			{
				strcpy(j->id, i->id);
				j->flag = 1;
			}
			else
			{
				l = (struct freetable *)malloc(sizeof(struct freetable));
				temp2 = j->size;
				j->size = i->size;
				strcpy(j->id, i->id);
				l->addr = j->addr + i->size;
				l->size = temp3 - i->size;

				if (l->size != 0)
				{
					l->next = j->next;
					l->flag = 0;
					strcpy(l->id, "¿ÕÏÐÇø");
					j->flag = 1;
					j->next = l;
				}
			}
		}
		head = head->next;
	}

	return;
}
//Recycle memory
void callback(struct freetable *e)
{
	struct freetable *head, *i, *j;
	char name[10];
	int m = 0;
	head = e;
	i = e;

	printf("\n\nPleace input recycle process name:");
	gets(name);
	while (head != NULL)//search for this process
	{
		if (head->flag == 1 && strcmp(head->id, name) == 0)
		{
			m = 1;
			strcpy(head->id, "¿ÕÏÐÇø");
			head->flag = 0;
			break;
		}
		head = head->next;
	}
	if (m == 0)
	{
		printf("\nCannot find target process!");
		return;
	}
	j = head->next;
	if (j->flag == 0)
	{
		head->next = j->next;
		head->size = head->size + j->size;
	}
	if (i->next != head)  //find the address before this target process
	{
		i = i->next;
	}
	if (i->flag == 0)
	{
		i->next = head->next;
		i->size = i->size + head->size;
	}
	return;
}
//Show memory
void showmem(struct freetable *a)
{
	struct freetable *head;
	char sg[10];
	head = a;
	printf("\n\n\t  Memory allocate table£º\n");

	while (head != NULL)
	{
		if (head->flag == 0) strcpy(sg, "Not Allocated");
		if (head->flag == 1) strcpy(sg, "Allocated");
		printf("\nProcess name:%s\tStarting address:%d\t  Length:%d\t  Status:%s", head->id, head->addr, head->size, sg);
		head = head->next;
	}
	printf("\n");
}
//Main
void initial()
{
	struct freetable *p, *q;

	p = (struct freetable *)malloc(sizeof(struct freetable));
	strcpy(p->id, "os");
	p->addr = 0;
	p->size = 40;
	p->flag = 1;
	p->next = NULL;
	head = p;
	p = (struct freetable *)malloc(sizeof(struct freetable));
	strcpy(p->id, "job1");
	p->addr = 40;
	p->size = 130;
	p->flag = 1;
	p->next = NULL;
	head->next = p;
	q = p;
	p = (struct freetable *)malloc(sizeof(struct freetable));
	strcpy(p->id, "job2");
	p->addr = 170;
	p->size = 60;
	p->flag = 1;
	p->next = NULL;
	q->next = p;
	q = q->next;
	p = (struct freetable *)malloc(sizeof(struct freetable));
	strcpy(p->id, "¿ÕÏÐÇø");
	p->addr = 230;
	p->size = 38;
	p->flag = 0;
	p->next = NULL;
	q->next = p;
	q = q->next;
	p = (struct freetable *)malloc(sizeof(struct freetable));
	strcpy(p->id, "¿ÕÏÐÇø");
	p->addr = 268;
	p->size = 12;
	p->flag = 0;
	p->next = NULL;
	q->next = p;
	q = q->next;
	p = (struct freetable *)malloc(sizeof(struct freetable));
	strcpy(p->id, "job3");
	p->addr = 280;
	p->size = 100;
	p->flag = 1;
	p->next = NULL;
	q->next = p;
	q = q->next;
	p = (struct freetable *)malloc(sizeof(struct freetable));
	strcpy(p->id, "¿ÕÏÐÇø");
	p->addr = 380;
	p->size = 96;
	p->flag = 0;
	p->next = NULL;
	q->next = p;
	q = q->next;
	p = (struct freetable *)malloc(sizeof(struct freetable));
	strcpy(p->id, "¿ÕÏÐÇø");
	p->addr = 476;
	p->size = 39;
	p->flag = 0;
	q->next = NULL;
	q->next = p;
	q = q->next;
	p = (struct freetable *)malloc(sizeof(struct freetable));
	strcpy(p->id, "¿ÕÏÐÇø");
	p->addr = 515;
	p->size = 85;
	p->flag = 0;
	p->next = NULL;
	q->next = p;
	q = q->next;

}
