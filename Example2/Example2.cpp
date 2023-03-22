#include <stdio.h>
#include <mpi.h>
#include <iostream>

using namespace std;

int main(int* argc, char** argv)
{
	int numtasks, rank,r;
	int sum=0;
	MPI_Status status;
	double t1 = 0, t2 = 0;
	
	MPI_Init(argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	t1 = MPI_Wtime();

	if (rank!=0) MPI_Send(&rank, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
	else
	{
		for (int i = 1; i < numtasks; i++)
		{
			MPI_Recv(&r, 1, MPI_INT, i, i, MPI_COMM_WORLD, &status);
			sum += r;
		}
	}
	t2 = MPI_Wtime();
	//MPI_Reduce(&rank, &r, 1, MPI_INT,MPI_SUM,0, MPI_COMM_WORLD);
	
	//printf("rank=%d, Result=%d\n", rank ,sum);
	//printf("Time=%g", t2 - t1);
	cout << t2 - t1;
	MPI_Finalize();
}