#include <stdio.h>
#include <mpi.h>
#include <iostream>

using namespace std;

int main(int* argc, char** argv)
{
	int numtasks, rank, i = 1,b=2;
	MPI_Request request1;
	MPI_Request request2;
	MPI_Status status;
	double t1 = 0, t2 = 0;

	MPI_Init(argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	t1 = MPI_Wtime();
	MPI_Isend(&i, 1, MPI_INT, (rank + 1) % numtasks, rank, MPI_COMM_WORLD,&request1);
	MPI_Irecv(&b, 1, MPI_INT, (rank + numtasks-1) % numtasks, (rank + numtasks-1) % (numtasks), MPI_COMM_WORLD, &request2);

	MPI_Wait(&request1, &status);
	MPI_Wait(&request2, &status);
	//printf("Tag =%d, b=%d\n", status.MPI_TAG,b);
	t2 = MPI_Wtime();
	//printf("Time=%g", t2 - t1);
	cout << t2 - t1;
	MPI_Finalize();
}