#include <stdio.h>
#include <mpi.h>

using namespace std;

int main(int* argc, char** argv)
{
	int numtasks, rank,sum=0,r;
	char i = 'a';
	MPI_Status status;
	MPI_Init(argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	if (rank == 0)
	{
		
		MPI_Ssend(&rank, 1, MPI_INT, (rank + 1) % numtasks, rank, MPI_COMM_WORLD);
		MPI_Recv(&r, 1, MPI_INT, (rank + numtasks - 1) % numtasks, (rank + numtasks - 1) % (numtasks), MPI_COMM_WORLD, &status);

	}
	else
	{
		MPI_Recv(&r, 1, MPI_INT, (rank + numtasks - 1) % numtasks, (rank + numtasks - 1) % (numtasks), MPI_COMM_WORLD, &status);
		sum = r+rank;
		MPI_Ssend(&sum, 1, MPI_INT, (rank + 1) % numtasks, rank, MPI_COMM_WORLD);
	}
	printf("Hello MPI from process =%d, result: %d\n", rank, r);
	MPI_Finalize();
}