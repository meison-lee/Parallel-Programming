#include<mpi.h>
#include<stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char* argv[]) {
	int id;// id of proc
	int size;// number of proc
	double elapsed_time;
	double elapsed_timee;
	long int m; //length of message
	MPI_Status s;
	int i;
	int j;
	int count;// number of loop
	int r;
	char a;

	// data from single test
	float resultt;
	float result;//transfer time
	float MPTime;
	float band;//bandwidth
	float a_band;
  float latency;





	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Barrier(MPI_COMM_WORLD);
	count = 27;
	m = 1;

  // let two process communticate for 1000 times
	for (i = 0; i < 1000; i++) {
		if (id == 0) {
			elapsed_timee = -MPI_Wtime();
			MPI_Send(&a, m, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
			MPI_Recv(&a, m, MPI_CHAR, 1, 1, MPI_COMM_WORLD, &s);
			elapsed_timee += MPI_Wtime();
		}
		else if (id == 1) {
			MPI_Recv(&a, m, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &s);
			MPI_Send(&a, m, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
		}

		resultt += elapsed_timee;
		// elapsed_time = 0;

	}
	MPTime = resultt / 2000.0;


  elapsed_time = 0;

	for (j = 0; j <= count; j++) {

    // bit shift operator
		m = 1 << j;

		double *A =malloc(m * sizeof(double));//allocate memory for array, size=m
		for (r = 0; r < m; r++) {
			A[r] = 0.0;// initialize A
		}
		if (id == 0) {
			printf("Tranfer size(Byte): %10ld  ",8*m);// one double contain 8 byte
			fflush(stdout);
		}
		for (i = 0; i < 25; i++) {
			if (id == 0) {
				elapsed_time = -MPI_Wtime();
				MPI_Send(A, m, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
				MPI_Recv(A, m, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD, &s);
				elapsed_time += MPI_Wtime();
			}
			else if(id==1) {
				MPI_Recv(A, m, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &s);
				MPI_Send(A, m, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
			}

			result += elapsed_time;
			elapsed_time = 0;

		}
		band =((8*m-1) / (result / 50.0 - MPTime))/pow(2,30);// after we have MPTime, we also acquire bandwidth because of the transfer time equation.

		if (!id) {
			printf("Transfer time (s): %9.7f  "
				"MPTime (s): %9.7f  "
				"band width (GB/s): %9.7f \n", result / 50.0, MPTime, band);
        printf("latency = %9.7f \n", result/50.0 - (8*m-1)/band/pow(2, 30));
			fflush(stdout);
      latency += result/50.0 - (8*m-1)/band/pow(2, 30);
			a_band += band;
		}
		result = 0;
		free(A);
	}




	MPI_Finalize();

	if (!id) {
		a_band /= count;
    latency /= count;
		printf("Average band width(Gb/s): %9.7f"
			"  Latency(s): %9.7f", a_band,latency);

	}

	return 0;
}
