#include<mpi.h>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>


#define EMPTY_MSG 0
#define NUMBER_MSG 1
#define RANGE_MSG 2



 int isprime(unsigned long long int i) {
	unsigned long long int j;
	for (j = 2; j * j < i; j++) {
		if (i % j == 0) return 0;
	}
	return 1;
}

void sort(unsigned long long int* result) {
	unsigned long long int small;
	int i, j;
	int tmp;
	for (j = 0; j < 7; j++) {
		small = 9223372036854775807;
		for (i = j; i < 8; i++) {
			if (result[i] < small) {
				small = result[i];
				tmp = i;
			}
		}
		result[tmp] = result[j];
		result[j] = small;
	}
}

void manager(int argc,char* argv[],int p) {
	unsigned long long int end=0;
	int count=0; /*numer of finished job*/
	unsigned long long int range=2; /*content of work*/
	int src;
	int tag;
	int i,j,k;
	int terminated=0; /*count of terminated processors*/
	MPI_Status status;
	unsigned long long int result[8];
	unsigned long long int buffer;
	double sorttime;
	double fortime;
	double printtime;

	fortime = -MPI_Wtime();
	do {
		MPI_Recv(&buffer, 1, MPI_UNSIGNED_LONG_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		src = status.MPI_SOURCE;
		if (buffer != 0) {
				result[count] = buffer;
				count += 1;
		}
		if (count < 8) {
			MPI_Send(&range, 1, MPI_UNSIGNED_LONG_LONG, src, RANGE_MSG, MPI_COMM_WORLD);
			range += 1;
		}
		else {
			MPI_Send(&end, 1, MPI_UNSIGNED_LONG_LONG, src, RANGE_MSG, MPI_COMM_WORLD);
			terminated += 1;	
		}
	} while (terminated < (p - 1));
	fortime += MPI_Wtime();
	sorttime = -MPI_Wtime();
	sort(result);
	sorttime += MPI_Wtime();
	printf("First eight perfect number: ");
	printtime = -MPI_Wtime();
	for (j = 0; j < 8; j++) {
		printf("%llu ", result[j]);
	}
	printtime += MPI_Wtime();
	printf("\nsorting time:%.15lf while loop time:%.10lf print time:%10lf ",sorttime,fortime,printtime);
}

void worker(int argc, char* argv[]) {
	unsigned long long int range;
	unsigned long long int range1;
	unsigned long long int range2;
	unsigned long long int result;
	
	int i,j,k;
	MPI_Status status;


	result = 0;
	/*initial request*/
	MPI_Send(&result,1, MPI_UNSIGNED_LONG_LONG, 0, EMPTY_MSG, MPI_COMM_WORLD);

	for (;;) {
		MPI_Recv(&range, 1, MPI_UNSIGNED_LONG_LONG, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		if (range == 0) {
			break;
		}
		range1 = range / 2;
		range2 = range - range1;
		if (isprime(pow(2,range)-1) == 1) result = (pow(2,range)-1)*pow(2,range-1);
		else result = 0;
		MPI_Send(&result, 1, MPI_UNSIGNED_LONG_LONG, 0, NUMBER_MSG, MPI_COMM_WORLD);
	}
	
}

int main(int argc, char* argv[]) {
	int p;
	int id;
	double time;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Barrier(MPI_COMM_WORLD);
	time = -MPI_Wtime();
	if (!id) {
		manager(argc, argv,p);
		time += MPI_Wtime();
		printf("\n total execution time: %f", time);
	}
	else{
		worker(argc,argv);
	}
	MPI_Finalize();
	return 0;
}
