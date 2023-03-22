#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

using namespace std;

int main(int* argc, char** argv)
{
	int numtasks, rank, r, a,b;
	int sum = 0,sum2=0;
	MPI_Status status;
	MPI_Init(argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	double t1=0,t2=0;
	//a = sqrt(numtasks);
	a =2;
	b = 4;
	t1 = MPI_Wtime();
	if (rank != 0 && rank % a != 0)
	{
		sum = rank;
		if ((rank % a) != a - 1)
		{
			MPI_Recv(&r, 1, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD, &status);
			
			sum = r+rank;
			//printf("Rang=%d,Summa=%d, tag=%d", rank,sum, status.MPI_TAG);
		}
		MPI_Send(&sum, 1, MPI_INT, rank - 1, rank, MPI_COMM_WORLD);
		
	}
	else
	{
		MPI_Recv(&r, 1, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD, &status);
		sum =r+rank;
		
		if (rank!=numtasks-a)
		{
			MPI_Recv(&r, 1, MPI_INT, rank + a, rank + a, MPI_COMM_WORLD, &status);

			sum += r;
		}
		if (rank!=0) MPI_Send(&sum, 1, MPI_INT, rank - a, rank, MPI_COMM_WORLD);
		
		//printf("Rang = % d, sum=%d, tag = % d",rank,sum, status.MPI_TAG);
		
	}
	t2 = MPI_Wtime();
	//printf("Time=%g",t2 - t1);
	cout << t2 - t1;
	MPI_Finalize();
}