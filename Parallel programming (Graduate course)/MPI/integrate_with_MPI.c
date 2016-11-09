#include <stdio.h>
#include <math.h>
#include "mpi.h"
#include <time.h>
#define PI 3.141592635

int main(int argc, char **argv) 
{ 
  int my_rank;
  int p;  /* my process rank */
  long long num_intervals;
  double local_a;  /*lower bound for each process*/
  double  local_b; /*upper bound for each process*/
  long long  local_n; /*elements for each process*/
  double rect_width, area, sum, x_middle; 
  double my_sum;
  double time;
  clock_t start,finish;
  double Get_sum(double local_a,double local_b,double local_n,double rect_width);
  
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&p);

  if (argc!=2){ 
	 if(my_rank==0)
     printf("please just enter one integer!\n"); 
     /* Command-line is not one argument. */
     MPI_Finalize();
	 return 0;
  }
  
  if(my_rank==0){
  sscanf(argv[1],"%llu",&num_intervals);
  }

  start=clock();
  MPI_Bcast(&num_intervals,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
  
  rect_width = PI / num_intervals;
  
  local_n=num_intervals/p;
  
  local_a = 1 + my_rank*local_n; /* lower bound */
  local_b = local_a + local_n; /* upper bound */
  my_sum=Get_sum(local_a,local_b, local_n, rect_width);


  MPI_Reduce(&my_sum,&sum,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
  finish=clock();

  if(my_rank==0){
  time=(double)(finish-start)/CLOCKS_PER_SEC;
  printf("The total area is: %f\n",(float)sum);
  printf("Total time:%fs\n",(float)time);
  } 
  MPI_Finalize();
 
} /* main*/  

  double Get_sum(double local_a,double local_b,double local_n,double rect_width)
{ 
  double x_middle;
  double my_sum = 0;
  double area;
  long long i;
  for(i = local_a; i < local_b; i++) {

    /* find the middle of the interval on the X-axis. */ 
    x_middle = (i - 0.5) * rect_width;
    area = sin(x_middle) * rect_width; 
    my_sum = my_sum + area;
  }
  return my_sum; 
}

