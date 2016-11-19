#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#define MAX 5 
pthread_t ntid_p;
pthread_t ntid_c;
int Buf[MAX];
sem_t Full, Empty, Mutex;
void * Producer(void *arg)
{
	int i = 0;
	int in = 0;
	while (i<10)
	{
		//sleep(1);
		sem_wait(&Empty);
		sem_wait(&Mutex);
		Buf[in] = i;
		printf("Producing...Buf[%d]=[%d]\n", in, Buf[in]);
		in = (in + 1) % MAX;
		i++;
		sem_post(&Full);
		sem_post(&Mutex);

	}
	return((void *)0);
}
void * Consumer(void *arg)
{
	int j = 0;
	int out = 0;

	while (j<10)
	{
		sem_wait(&Full);
		sem_wait(&Mutex);

		printf("\t Consuming...Buf[%d]=[%d]\n", out, Buf[out]);
		Buf[out] = -1;
		out = (out + 1) % MAX;
		j++;

		sem_post(&Empty);
		sem_post(&Mutex);
	}
	return ((void *)0);
}
int main(void)
{
	int err_p, err_c;
	int k;
	sem_init(&Full, 0, 0);
	sem_init(&Empty, 0, MAX);
	sem_init(&Mutex, 0, 1);

	for (k = 0; k<MAX; k++)
		Buf[k] = -1;
	err_p = pthread_create(&ntid_p, NULL, Producer, NULL);
	if (err_p != 0)
		printf("can't create thread: %s\n", strerror(err_p));
	err_c = pthread_create(&ntid_c, NULL, Consumer, NULL);
	if (err_c != 0)
		printf("cannot create thread %s\n", strerror(err_c));

	pthread_join(ntid_p, NULL);
	pthread_join(ntid_c, NULL);
	//while(!getchar());
	return 0;
}