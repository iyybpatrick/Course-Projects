#include<sys/sysinfo.h>
#include <pthread.h>
#include<iostream>
#include<time.h>
#include<sys/time.h>
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
using namespace std;

/* here you can change the number of thread by altering this parameter  */
int N_thread_count= get_nprocs(); 

pthread_mutex_t mutex;
int thread_count;
long number_of_tosses;
long sum_toss;

void* Thread_sum(void* rank)
        {
	int  my_rank = (long)rank;
	double x, y, distance_squared;
	long i;
	long my_n = number_of_tosses / thread_count;
	long my_first_i = my_n * my_rank;
	long my_last_i = my_first_i + my_n;
	long my_cnt = 0;
	unsigned seed =(unsigned)(time)(NULL);
	for (i = my_first_i; i < my_last_i; i++)
        {
	  x =rand_r(&seed)/(double)RAND_MAX; 
	y =rand_r(&seed)/(double)RAND_MAX;
		distance_squared = x*x + y*y;
		if (distance_squared <= 1)
		{
			my_cnt++;
		}
	}
	pthread_mutex_lock(&mutex);
	sum_toss += my_cnt;
	pthread_mutex_unlock(&mutex);
	return NULL;
        }  /* Thread_sum */
int main(int argc, char* argv[]) {
	double pi_estimate;
	clock_t start, finish;
	long thread;
 /* Use long in case of a 64-bit system */
	pthread_t* thread_handles;
	thread_count = N_thread_count;
	number_of_tosses =strtoll(argv[1], NULL, 10);
	thread_handles = (pthread_t*)malloc(thread_count*sizeof(pthread_t));
	pthread_mutex_init(&mutex, NULL);
	sum_toss = 0.0;
	start=clock();
	for (thread = 0; thread < thread_count; thread++)
		pthread_create(&thread_handles[thread], NULL,
		Thread_sum, (void*)thread);
       
	for (thread = 0; thread < thread_count; thread++)
		pthread_join(thread_handles[thread], NULL);
        finish=clock();
	pi_estimate = 4 * sum_toss / ((double)number_of_tosses);
        
	cout << pi_estimate <<endl;
	pthread_mutex_destroy(&mutex);
	free(thread_handles);

	double totaltime;
	totaltime = (double)(finish - start) * 1000 / CLOCKS_PER_SEC;        
	cout << totaltime << "ms" << endl;

	return 0;
} /* main */
