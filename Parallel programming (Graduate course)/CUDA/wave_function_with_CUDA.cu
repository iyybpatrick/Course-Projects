/**********************************************************************
* DESCRIPTION:
* Serial Concurrent Wave Equation - C Version
* This program implements the concurrent wave equation
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define MAXPOINTS 1000000
#define MAXSTEPS 1000000
#define MINPOINTS 20
#define PI 3.14159265
#define THREAD_NUM 512
#define BLOCK_NUM 64
void check_param(void);
void init_line(void);
void update (void);
void printfinal (void);
int nsteps , /* number of time steps */
tpoints , /* total points along string */
rcode; /* generic return code */
float values[MAXPOINTS+2], /* values at time t */
oldval[MAXPOINTS+2], /* values at time (t-dt) */
newval[MAXPOINTS+2]; /* values at time (t+dt) */
/**********************************************************************
* Checks input values from parameters
*********************************************************************/
void check_param(void)
{
char tchar[20];
/* check number of points , number of iterations */
while ((tpoints < MINPOINTS) || (tpoints > MAXPOINTS)) {
printf("Enter number of points along vibrating string [%d-%d]: "
,MINPOINTS , MAXPOINTS);
scanf("%s", tchar);
tpoints = atoi(tchar);
if ((tpoints < MINPOINTS) || (tpoints > MAXPOINTS))
printf("Invalid. Please enter value between %d and %d\n",
MINPOINTS , MAXPOINTS);
}
while ((nsteps < 1) || (nsteps > MAXSTEPS)) {
printf("Enter number of time steps [1-%d]: ", MAXSTEPS);
scanf("%s", tchar);
nsteps = atoi(tchar);
if ((nsteps < 1) || (nsteps > MAXSTEPS))
printf("Invalid. Please enter value between 1 and %d\n",
MAXSTEPS);
}
printf("Using points = %d, steps = %d\n", tpoints , nsteps);
}
/**********************************************************************
* Initialize points on line
*********************************************************************/
void init_line(void)
{
int i, j;
float x, fac, k, tmp;
/* Calculate initial values based on sine curve */
fac = 2.0 * PI;
k = 0.0;
tmp = tpoints - 1;
for (j = 1; j <= tpoints; j++) {
x = k/tmp;
values[j] = sin (fac * x);
k = k + 1.0;
}
/* Initialize old values array */
for (i = 1; i <= tpoints; i++)
oldval[i] = values[i];
}
/**********************************************************************
* Calculate new values using wave equation
*********************************************************************/
__global__ static void do_math(float *oldval, float*values, float *newval, int *nsteps1, int *tpoints1)
{
	const int tid = threadIdx.x;
	const int bid = blockIdx.x;
	int i, j;

        int nsteps=nsteps1[0];
        int tpoints=tpoints1[0];
       /* 
        float *oldval = new float[tpoints];
	float *values = new float[tpoints];
	float *newval = new float[tpoints];

	oldval = oldval1;
	values = values1;
	newval = newval1;
       */
	float dtime, c, dx, tau, sqtau;
	dtime = 0.3;
	c = 1.0;
	dx = 1.0;
	tau = (c * dtime / dx);
	sqtau = tau * tau;

	for (i = 1; i <= nsteps; i++) {
		for (j = bid*THREAD_NUM + tid; j <= tpoints; j += BLOCK_NUM*THREAD_NUM)
		{

			newval[j] = (2.0 * values[j]) - oldval[j] + (sqtau * (-2.0)*values[
				j]);
				oldval[j] = values[j];
				values[j] = newval[j];
		}
		if (tid == 0)
		{
			values[0] = 0.0;
			newval[0] = 0.0;
		}
		if (tid == BLOCK_NUM)
		{
			values[tpoints] = 0.0;
			newval[tpoints] = 0.0;
		}
            	//	__syncthreads();
	}

}

/**********************************************************************
* Print final results
*********************************************************************/
void printfinal()
{
int i;
for (i = 1; i <= tpoints; i++) {
printf("%6.4f ", values[i]);
if (i%10 == 0)
printf("\n");
}
}
/**********************************************************************
* Main program
*********************************************************************/
int main(int argc, char *argv[])
{

     //	clock_t start, finish;
        
	sscanf(argv[1], "%d", &tpoints);
	sscanf(argv[2], "%d", &nsteps);
	check_param();
	printf("Initializing points on the line...\n");
	init_line();
	printf("Updating all points for all time steps...\n");

     //	start = clock();
	int DATA_SIZE = tpoints;

	/*Update function*/
	int *gpu_oldval, *gpu_values, *gpu_newval,*nsteps1,*tpoints1;
	cudaMalloc((void**)&gpu_oldval, sizeof(float)*DATA_SIZE);
	cudaMalloc((void**)&gpu_values, sizeof(float)*DATA_SIZE);
	cudaMalloc((void**)&gpu_newval, sizeof(float)*DATA_SIZE);
        cudaMalloc((void**)&nsteps1,sizeof(int)*1);
        cudaMalloc((void**)&tpoints1,sizeof(int)*1);


	cudaMemcpy(gpu_oldval, oldval, sizeof(float)*DATA_SIZE, cudaMemcpyHostToDevice);
	cudaMemcpy(gpu_values, values, sizeof(float)*DATA_SIZE, cudaMemcpyHostToDevice);
	cudaMemcpy(gpu_newval, newval, sizeof(float)*DATA_SIZE, cudaMemcpyHostToDevice);
        cudaMemcpy(nsteps1,&nsteps,sizeof(int)*1,cudaMemcpyHostToDevice);
        cudaMemcpy(tpoints1,&tpoints,sizeof(int)*1,cudaMemcpyHostToDevice);
        

	do_math <<< BLOCK_NUM, THREAD_NUM >>>((float*)gpu_oldval,(float*) gpu_values,(float*) gpu_newval,nsteps1,tpoints1);

	cudaMemcpy(values, gpu_values, sizeof(float)*DATA_SIZE, cudaMemcpyDeviceToHost);


	cudaFree(gpu_oldval);
	cudaFree(gpu_values);
	cudaFree(gpu_newval);


	printf("Printing final results...\n");
	printfinal();
	printf("\nDone.\n\n");
     //	finish = clock();
     //	printf("time:%f\n", (float)(finish - start) /( CLOCKS_PER_SEC));
	return 0;
}
