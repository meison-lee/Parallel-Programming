#include<mpi.h>
#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#define n 50

double f(double i) {

	return 4.0 / (1.0 + i * i);
}


int main(int argc, char *argv[]) {

	int p;
	int id;
	double local_area;
	double global_area;
	int j;
	double elapsed_time;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Barrier(MPI_COMM_WORLD);
	//start the timer
	elapsed_time=-MPI_Wtime();

	//assign X1 to proc 0, X2 to proc 1....
	for (j = (id + 1); j <= n; j += p) {
		// from the equation we know all odd X need to mutiply 4, and even X need to multiply 2
		if (j % 2 == 0) {
			local_area += 2.0 * f((double)j / (double)n);
		}
		else {
			local_area += 4.0 * f((double)j / (double)n);
		}
	}


	MPI_Reduce(&local_area, &global_area, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	global_area = (global_area+f(0.0)-f(1.0)) / (double)(3 * n);
	//stop the timer
	elapsed_time += MPI_Wtime();
	MPI_Finalize();
	if (!id) {

		printf("Approximation of pi: %13.11f\n"
			"elapsed time (s): %f\n", global_area,elapsed_time);
		fflush(stdout);
		return 0;
	}

}
