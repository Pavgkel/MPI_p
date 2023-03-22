#include <stdio.h>
#include <mpi.h>

using namespace std;

int main(int* argc, char** argv)
{
	int numtasks, rank;
	char i = 'a';
	MPI_Status status;
	MPI_Init(argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	MPI_Send(&i, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	MPI_Recv(&i, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

	printf("Hello MPI from process =%d, total number of process: %d\n", rank, status);
	MPI_Finalize();
}