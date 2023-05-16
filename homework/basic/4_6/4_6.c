#include "mpi.h"
#include <stdio.h>

int main (int argc, char *argv[]) {

   MPI_Init (&argc, &argv);

   int id;               /* Process rank */
   int p;
   double elapsed_time;


   MPI_Comm_size (MPI_COMM_WORLD, &p);
   MPI_Comm_rank (MPI_COMM_WORLD, &id);
   elapsed_time = -MPI_Wtime();


   printf ("hello, world, from process %d\n", id);
   fflush (stdout);
   elapsed_time += MPI_Wtime();

   if (!id){
      printf("elapsed time = %d\n", elapsed_time);
   }
   MPI_Finalize();
   return 0;
}
