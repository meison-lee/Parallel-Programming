#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<mpi.h>

#define BLOCK_LOW(id, p, n) ((id)*(n)/(p))
#define BLOCK_HIGH(id, p, n)  (((id + 1)*n/p)-1)
#define BLOCK_SIZE(id, p, n)  (BLOCK_HIGH(id, p, n) - BLOCK_LOW(id, p, n)+ 1)
#define BLOCK_OWNER(index, p, n)  (((p) * (index+1) - 1) / n)

int main(int argc, char *argv[]){

  int i = 0;
  int id;
  int p;
  float global;
  float elapsed_time;
  int n = 1000000;
  int low_value, high_value, size, proc0_size;
  int* marked;
  int index, prime, first;
  int Local_largest_gap = 0;
  int lastPrime = 0, firstPrime = n;
  int recvNum = 0;
  int Global_largest_gap;


  MPI_Init (&argc, &argv);
  MPI_Barrier(MPI_COMM_WORLD);
  elapsed_time = -MPI_Wtime();

  MPI_Comm_size (MPI_COMM_WORLD, &p);
  MPI_Comm_rank (MPI_COMM_WORLD, &id);

  low_value = 2 + BLOCK_LOW(id, p, n);
  high_value = 2 + BLOCK_HIGH(id, p, n);
  size = BLOCK_SIZE(id, p, n);
  proc0_size = (n-1)/p;

  if ((proc0_size + 1) < (int) sqrt((double)n)){
    if (!id){
      printf("too many processor\n");
      MPI_Finalize();
      // exit(1);
    }
  }

  marked = (int*)malloc(size * sizeof(MPI_INT));
  if (marked == NULL){
    printf("not enough memmory to allocate\n");
    MPI_Finalize();
    // exit(1);
  }
  for (i  = 0; i < size; i++){
    marked[i] = 0;
  }

  if (!id) index = 0;
  prime = 2;

  do {
    // find the first element in each process
    if (prime * prime > low_value){
      first = prime* prime - low_value;
    }
    else{
      if (!(low_value % prime)) first = 0;
      else first = prime - (low_value % prime);
    }
    for (i = first ; i < size; i += prime) marked[i] = 1;

    if (!id){
      // marked[index] is prime
      while(marked[++index]);
      prime = index + 2;
    }
    MPI_Bcast(&prime, 1, MPI_INT, 0, MPI_COMM_WORLD);
  }while(prime * prime <= n );

  printf("process %d is running ", id);
  for (i = 0; i < size; i++){
    if (marked[i] == 0){
      if (lastPrime){
        if (((low_value + i) - lastPrime) > Local_largest_gap){
          Local_largest_gap = (low_value + i) - lastPrime;
        }
      }
      firstPrime = fmin(firstPrime, low_value + i);
      lastPrime = low_value + i;
      printf("%d ", lastPrime);
    }
  }

  printf("\nid = %d, LLG = %d\n", id, Local_largest_gap);

  if (id < p -1){
    MPI_Send(&lastPrime, 1, MPI_INT, id + 1, id + 1, MPI_COMM_WORLD);
  }
  if (id){
    MPI_Recv(&recvNum, 1, MPI_INT, id -1, id, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
  if (recvNum){
    if ((firstPrime - recvNum) > Local_largest_gap){
      Local_largest_gap = firstPrime - recvNum;
    }
  }
  printf("id = %d, LLG = %d\n", id, Local_largest_gap);
  MPI_Reduce(&Local_largest_gap,  &Global_largest_gap, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);

  elapsed_time += MPI_Wtime();

  if (!id){
    printf("global largest gap = %d\n", Global_largest_gap);
    printf("time = %f", elapsed_time);
  }

  MPI_Finalize();
  return 0;
}