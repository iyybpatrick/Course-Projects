#include <stdio.h>
#include <stdlib.h>		
#include <time.h>		
#include <sys/sysinfo.h>
#include <pthread.h>

typedef struct thread_data {
	double *in_circle;
	pthread_mutex_t *in_circle_lock;
	int threadID;
	long long num_tosses;
}thread_data;

void *toss_thread(void *threadD);

int main(int argc, char const *argv[])
{
	int i;
	double in_circle = 0;
	long long number_of_tosses;
	if (argc == 2) {
		number_of_tosses = atoi(argv[1]);
	}
	else {
		number_of_tosses = 100000001;
	}
	int NUM_OF_THREADS = get_nprocs();
	long long tosses_of_thread = number_of_tosses / NUM_OF_THREADS;
	long long tosses_of_remain = number_of_tosses % NUM_OF_THREADS;
	pthread_t* threads;
	thread_data* threadD;
	pthread_mutex_t in_circle_lock;
	threads = (pthread_t*)malloc(sizeof(pthread_t)* NUM_OF_THREADS);
	threadD = (thread_data*)malloc(sizeof(thread_data)* NUM_OF_THREADS);
	pthread_mutex_init(&in_circle_lock, NULL);
	for (i = 0; i < NUM_OF_THREADS; i++) {
		threadD[i].in_circle = &in_circle;
		threadD[i].in_circle_lock = &in_circle_lock;
		threadD[i].threadID = i;
		if (i == 0) threadD[i].num_tosses = tosses_of_thread + tosses_of_remain;
		else threadD[i].num_tosses = tosses_of_thread;
		pthread_create(&threads[i], NULL, toss_thread, (void*)&threadD[i]);
	}
	for (i = 0; i < NUM_OF_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	double pi_estimate = 4 * in_circle / (double)number_of_tosses;
	printf("%.15lf\n", pi_estimate);
	return 0;
}
void *toss_thread(void *threadD)
{
	long long toss;
	double x, y;
	double distance_square;
	long long local_in_circle = 0;
	unsigned seed = (unsigned)time(NULL);
	thread_data* tData = (thread_data*)threadD;
	for (toss = 0; toss < tData->num_tosses; toss++) {
		x = 2 * (rand_r(&seed) / (double)RAND_MAX) - 1;
		y = 2 * (rand_r(&seed) / (double)RAND_MAX) - 1;
		distance_square = x*x + y*y;
		if (distance_square <= 1.0) {
			local_in_circle++;
		}
	}
	pthread_mutex_lock(tData->in_circle_lock);
	*(tData->in_circle) += local_in_circle;
	pthread_mutex_unlock(tData->in_circle_lock);
	pthread_exit(NULL);
}