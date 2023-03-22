#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <math.h>
using namespace std;

int main(int* argc, char** argv)
{
	int numtasks, rank, r, a;
	int sum = 0;
	MPI_Status status1;
	MPI_Status status2;
	MPI_Request request1;
	MPI_Request request2;
	MPI_Init(argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	a = sqrt(numtasks);
	
		//for (int i = a; i > 0; i--)
		//{
			if (rank != 0 && rank % a != 0)
			{
				MPI_Isend(&rank, 1, MPI_INT, rank - 1, rank, MPI_COMM_WORLD,&request1);
				if ((rank%a)!=a-1) MPI_Irecv(&r, 1, MPI_INT, rank+1, rank+1, MPI_COMM_WORLD, &request2);
				MPI_Wait(&request1, &status1);
				sum += r;
				printf("Summa=%d, tag=%d", sum, status1.MPI_TAG);
			}
			else
			{
				MPI_Recv(&r, 1, MPI_INT, rank + 1, rank + 1, MPI_COMM_WORLD, &status2);
				//MPI_Wait(&request2, &status2);
				printf("tag = % d", status2.MPI_TAG);
			}
		//}
	MPI_Finalize();
}