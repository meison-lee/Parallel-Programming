#include "mpi.h"
#include <stdio.h>
#include<stdlib.h>

int main (int argc, char *argv[]) {

  MPI_Init (&argc, &argv);

  int id;               /* Process rank */
  int p;

  int count = 0;
  double elapsed_time;
  int send = 0;
  int recv = 0;
  int sendTag = 10;
  int recvTag = 20;

  MPI_Comm_size (MPI_COMM_WORLD, &p);
  MPI_Comm_rank (MPI_COMM_WORLD, &id);


  float result=0;
  for (int i = 0 ; i < 10; i++){
    if (!id){
      elapsed_time = - MPI_Wtime();
      MPI_Send(&send, 1, MPI_INT, 1, sendTag, MPI_COMM_WORLD);
      MPI_Recv(&recv, 1, MPI_INT, 1, recvTag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      elapsed_time += MPI_Wtime();
      result += elapsed_time;
    }
    else{
      MPI_Recv(&recv, 1, MPI_INT, 0, sendTag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Send(&recv, 1, MPI_INT, 0, recvTag, MPI_COMM_WORLD);
    }

    if (id == 0){
      printf("id = %d=> time = %.10f\n", id, elapsed_time);
      printf("result = %f\n", result);
      fflush(stdout);
    }
   }
   printf("avg = %f\n", result/10);
   MPI_Finalize();

   return 0;
}
