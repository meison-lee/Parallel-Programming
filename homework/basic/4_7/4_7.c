#include<stdio.h>
#include<mpi.h>

int main(int argc, char *argv[]){

  MPI_Init (&argc, &argv);

  int id;               /* Process rank */
  int p;
  float global;

  MPI_Comm_size (MPI_COMM_WORLD, &p);
  MPI_Comm_rank (MPI_COMM_WORLD, &id);

  float local = id + 1;
  MPI_Reduce(&local, &global, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (id == 0){
    printf("global = %f\n", global);
  }


  MPI_Finalize();
  return 0;
}