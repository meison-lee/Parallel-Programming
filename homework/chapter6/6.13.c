
#include <stdlib.h>
#include <stdio.h>
#include<mpi.h>
#include<math.h>

// determine whether next state is alive or dead
int determine(int status, int count) {
	if (status == 1) {
		if (count == 2 || count == 3) return 1;
		else return 0;
	}
	else {
		if (count == 3) return 1;
		else return 0;

	}

}
//print the matrix, * is alive
void print(int* A, int m, int n) {
	int i, j,k;
	for (k = 0; k < 2*n; k++) {
		printf("-");
	}
	printf("\n");
	for (i = 0; i < m ; i++) {
		for (j = 0; j < n ; j++) {
			if (A[j + i * n] == 1) printf("* ");
			else printf("  ");
		}
		printf("\n");
	}
	for (k = 0; k < 2 * n; k++) {
		printf("-");
	}
	printf("\n\n");

}

//determine whether next state is alve or dead
void check(int* A, int id, int n,int m,int qou,int remainder,int row_num, int start) {
	int* temp;
	int j;
	temp = malloc(m * n * sizeof(int));
	for (j = 0; j < m * n; j++) {
		temp[j] = A[j];
	}

	int i;
	// int start;
	int count=0;
	// if (row_num == qou) {
	// 	start = (row_num + 1) * n * remainder + qou * n * (id - remainder);

	// }
	// else {
	// 	start = (row_num * n * id);

	// }

	for (i = start; i < start+row_num * n; i++) {

		//left side
		if (i % n == 0) {
			//upper left corner
			if (i == 0) {
				count += temp[1];
				count += temp[n];
				count += temp[n + 1];
				A[i] = determine(temp[i], count);

			}
			//bottom left corner
			else if (i == (m - 1) * n) {
				count += temp[i - n];
				count += temp[i - n + 1];
				count += temp[i + 1];
				A[i] = determine(temp[i], count);

			}
			//left side middle part
			else {
				count += temp[i - n];
				count += temp[i - n+1];
				count += temp[i+1];
				count += temp[i + n];
				count += temp[i + n+1];
				A[i] = determine(temp[i], count);

			}
		}
		//right side
		else if (i % n == (n - 1)) {
			//upper right corner
			if (i == n - 1) {
				count += temp[i - 1];
				count += temp[i + n];
				count += temp[i + n - 1];
				A[i] = determine(temp[i], count);

			}
			//bottom right corner
			else if (i == m * n - 1) {
				count += temp[i - 1];
				count += temp[i - n];
				count += temp[i - n - 1];
				A[i] = determine(temp[i], count);

			}
			//right side middle part
			else {
				count += temp[i-1];
				count += temp[i+n];
				count += temp[i+n-1];
				count += temp[i-n];
				count += temp[i - n - 1];;
				A[i] = determine(temp[i], count);
			}
		}
		//upper side
		else if ((i>=1)&&(i<=n-2)) {
			count += temp[i - 1];
			count += temp[i + 1];
			count += temp[i+n];
			count += temp[i+n-1];
			count += temp[i+n+1];
			A[i] = determine(temp[i], count);

		}
		//bottom side
		else if (i >= ((m - 1) * n + 1) && i <= (m * n - 2)) {
			count += temp[i + 1];
			count += temp[i-1];
			count += temp[i-n];
			count += temp[i-n-1];
			count += temp[i-n+1];
			A[i] = determine(temp[i], count);

		}
		//middle part
		else {
			count += temp[i+1];
			count += temp[i-1];
			count += temp[i+n];
			count += temp[i-n];
			count += temp[i+n+1];
			count += temp[i+n-1];
			count += temp[i-n+1];
			count += temp[i-n-1];
			A[i] = determine(temp[i], count);

		}
		count = 0;
	}
	free(temp);

}





int main(int argc,char* argv[]) {
	int p;
	int id;
	MPI_Status s;
	double elapsed_time;

	double average;// total looping time
	double total_elapsed;
	double read;// reading matrix time
	double broad;// broadcast time
	double computation;// one loop time


	int iterations,k; // perfome for loop for "iteration" times, print the result every k moves.
	int m=0;
	int n=0;
	int* A; //store matrix
	int row_num; // the number of rows each processors be responsible for
	int remainder; // reaminder of m/p
	int qou; // floor(quotient of m/p)
	int i, j, count=0;
	int start; // first index of element each processors be responsible for

	int y;
	int u;

	char c;


	FILE* fp;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Barrier(MPI_COMM_WORLD);

	elapsed_time = -MPI_Wtime();
	total_elapsed = -MPI_Wtime();
	if (argv[1] == NULL || argv[2] == NULL) {
		iterations = 3;
		k = 1;
	}
	else {
		iterations = atoi(argv[1]);
		k = atoi(argv[2]);

	}


	//read the input matrix
	if (!id) {
		fp = fopen("matrix.txt", "r");

		// calculate the m and n first
		while (1) {
			// check end of file
			if (feof(fp)) {
				m += 1;
				break;
			}
			else {
				c = fgetc(fp);
				if (c == '\n') {
					if (m == 0) n = count;
					m += 1;
				}
				else if (c != ' ') count += 1;

			}
		}
		printf("n = %d, m = %d\n", n, m);
		fclose(fp);
		// allocate space for the 2D matrix
		A = malloc(m * n * sizeof(int));
		fp = fopen("matrix.txt", "r");

		//read the status of each grid
		while (1) {
			if (feof(fp)) {
				break;
			}
			else {
				for (i = 0; i < m; i++) {
					for (j = 0; j < n; j++) {
						fscanf(fp, "%d ", &A[j + i * n]);
					}
				}
			}
		}

		// print out the matrix
		for (i = 0; i < m; i++) {
			for (j = 0; j < n; j++) {
				printf("%d ", A[j + i * n]);
			}
			printf("\n");
		}
	}

	if (!id) {
		elapsed_time += MPI_Wtime();
		read = elapsed_time;
		printf("\nreading file time %f\n\n", elapsed_time);
	}


	// breoadcast the m and n to other processor
	elapsed_time = -MPI_Wtime();
	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);

	//initialize and broadcast the matirx for other processor
	if (id != 0) {
		A = malloc(m * n * sizeof(int));
	}
	MPI_Bcast(A, m * n, MPI_INT, 0, MPI_COMM_WORLD);
	if (!id) {
		elapsed_time += MPI_Wtime();
		broad = elapsed_time;
		printf("broadcast time %f\n\n", elapsed_time);
	}

	// allocate rows for processors
	// each processor get continuous grid
	remainder = m % p;
	qou = m / p;
	if (id < remainder) {
		row_num = qou+1;
		start = (qou + 1) * n * id;
	}
	else {
		row_num = qou;
		start = (qou + 1) * n * remainder+qou*n*(id-remainder);
	}
	if (!id) {
		printf("initial state: \n");
		print(A, m, n);
	}
	printf("id = %d row_num = %d start = %d \n", id, row_num, start);


	for (y = 0; y < iterations; y++) {
		elapsed_time = -MPI_Wtime();
		check(A, id, n, m, qou, remainder, row_num, start);

		// id 0 will collect the result from every processor
		// after collect, it will broadcast it to every processor
		if (!id) {
			for (u = 1; u < p; u++) {
				if (u < remainder) {
					MPI_Recv(&A[(qou + 1) * n * u], (qou + 1) * n, MPI_INT, u, u, MPI_COMM_WORLD, &s);
				}
				else {
					MPI_Recv(&A[(qou + 1) * n * remainder+qou * n * (u-remainder)], qou * n, MPI_INT, u, u, MPI_COMM_WORLD, &s);
				}
			}
			if ((y + 1) % k == 0) {
				printf("iteration %d:\n", y + 1);
				print(A, m, n);
			}
		}
		else {
			MPI_Send(&A[start], row_num * n, MPI_INT, 0, id, MPI_COMM_WORLD);
		}
		MPI_Bcast(A, m * n, MPI_INT, 0, MPI_COMM_WORLD);
		elapsed_time += MPI_Wtime();
		average += elapsed_time;
	}
	total_elapsed += MPI_Wtime();


	if (!id) {
		computation = average / (double)iterations;
		printf("one loop time =%f \n"
			"total elapsed time =%f\n"
			"estimated time =%f\n", average/iterations,total_elapsed,read+broad+computation*(double)iterations);
	}


MPI_Finalize();
}