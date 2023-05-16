#include "mpi.h"
#include<stdio.h>

int main(int argc, char *argv[]){
  MPI_Init(&argc, &argv);

  int id;
  int p;
  float global_sum = 0;

  MPI_Comm_size (MPI_COMM_WORLD, &p);
  MPI_Comm_rank (MPI_COMM_WORLD, &id);
  float local_sum = id+1;

  printf("%d's local number is %f\n", id, local_sum);
  fflush(stdout);

  MPI_Reduce(&local_sum, &global_sum, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (id == 0){
    printf("global_sum = %f\n", global_sum);
  }

  MPI_Finalize();
  return 0;
}