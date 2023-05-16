#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define BLOCK_LOW(id, p, n) ((id)*(n)/(p))
#define BLOCK_HIGH(id, p, n)  (((id + 1)*n/p)-1)
#define BLOCK_SIZE(id, p, n)  (BLOCK_HIGH(id, p, n) - BLOCK_LOW(id, p, n)+ 1)

int main(int argc, char *argv[]){

  int p, id;
  int n, d;
  int size;
  int i;
  int start;
  long double local = 0.0;
  long double global = 0.0;
  double elapsed_time;

  MPI_Init (&argc, &argv);
  MPI_Comm_size (MPI_COMM_WORLD, &p);
  MPI_Comm_rank (MPI_COMM_WORLD, &id);

  if (id == 0){
    scanf("%d", &n);
    scanf("%d", &d);
  }
  elapsed_time = -MPI_Wtime();

  MPI_Bcast(&n,1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&d, 1, MPI_INT, 0, MPI_COMM_WORLD);
  start = BLOCK_LOW(id, p, n);
  size = BLOCK_SIZE(id, p, n);

  for (i = 0; i < size; i++){
    printf("process %d => %d\n", id, start + i + 1);
    fflush (stdout);
    local += (long double)(1/(long double)(start + i + 1));
  }
  printf("process %d => local = %.*Lf\n", id, d,local);

  MPI_Reduce(&local, &global, 1, MPI_LONG_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  elapsed_time += MPI_Wtime();

  if (id == 0){
    printf("global sum = %.*Lf\n", d, global);
    printf("time = %f\n", elapsed_time);
  }
  MPI_Finalize();
}

// 2.592857122421