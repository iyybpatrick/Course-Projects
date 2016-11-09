#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <time.h>
int isprime(int n) {
  int i,squareroot;
  if (n>10) {
    squareroot = (int) sqrt(n);
    for (i=3; i<=squareroot; i=i+2)
      if ((n%i)==0)
        return 0;
    return 1;
  }
  else
    return 0;
}

int main(int argc, char *argv[])
{
  int pc,       /* total prime counter */
      foundone; /* most recent prime found */
  long long int n, limit;
  int my_rank; 
  int dest=0;
  int tag=0;
  int source;
  long long int local_a, local_b, local_n; 
  /* a and b are bounds, local_n is elements for each process */
  int p;  //num of processes
  int my_pc = 0;  /* num of primes for each process */
  double time;  
  clock_t start,finish;
  MPI_Status status;
  
  sscanf(argv[1],"%llu",&limit);
 

  if (limit <= 10)  /* The number to be scanned is less than 11 */
{  if(my_rank==0)
   printf("Wrong! Please enter a integer that is greater than 10!\n");
   exit(0); 
}	
  MPI_Init(&argc,&argv);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&p);
 
  if (argc != 2){    /* Command-line argument is not one */
  if(my_rank==0)
  printf("Wrong!Please enter just one element as input.\n");
  MPI_Finalize();
  exit(0);
} 
  start=clock();
  MPI_Bcast(&limit,1,MPI_LONG,0,MPI_COMM_WORLD);
  local_n = (limit - 10) / p;  /* elements for each process */
  local_a = 11 + my_rank*local_n;  /* lower bound */
  local_b = local_a + local_n;  /* upper bound */

  /* Only the last process will caculate 'foundone' */
  if (my_rank == p - 1)
{
  printf("Starting. Numbers to be scanned= %lld\n",limit);
  if (local_a % 2 == 0)   /* It's not odd */
     local_a++;
  for(n=local_a;n<=limit;n=n+2)
    {
	if(isprime(n))  {
		my_pc++;
		foundone=n; }
    }
}
  /*other process just caculate num of primes */
   else{
	   if ((local_a) % 2 == 0)    /*It's not odd */
   local_a++; 
  for(n=local_a;n<=local_b-1;n=n+2){
	if(isprime(n)){
		my_pc++;		
	}
    }
	
}
  MPI_Reduce(&my_pc,&pc,1,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
  finish=clock();
 
 if(my_rank==p-1){
   MPI_Send(&foundone,1,MPI_INT,0,2,MPI_COMM_WORLD);
 }
 if(my_rank==0){
   MPI_Recv(&foundone,1,MPI_INT,p-1,2,MPI_COMM_WORLD,&status);
   printf("Done. Largest prime is %d Total primes %d\n",foundone,pc+4);
   time=(double)(finish-start)/CLOCKS_PER_SEC;
   printf("Total time:%fs\n",(float)time);
} 
MPI_Finalize();
  return 0;
} 



