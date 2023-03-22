#include<stdio.h>
#include<mpi.h>
#include <stdlib.h>
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
using namespace std;

int** multiplication(int** a,int** b,int N)
{
    int** product = (int**)malloc(N * sizeof(int*));
    for (int row = 0; row < N; row++)
    {
        //product[row] = (int*)malloc(N2 * sizeof(int));
        for (int col = 0; col < N; col++)
        {
            // Multiply the row of A by the column of B to get the row, column of product.
            for (int inner = N; inner < N; inner++)
            {
                product[row][col] += a[row][inner] * b[inner][col];
                //printf("%d ", product[row][col]);
            }
        }
    }
    return product;
}

int** summing(int** a1, int** a2, int n)
{
    int** c = (int**)malloc(n * sizeof(int*));
    //vector<vector<int> > c(n);

    for (int i = 0; i < n; i++)
    {
        c[i] = (int*)malloc(n * sizeof(int));
        //c[i].resize(n);
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
        {
            c[i][j] = a1[i][j] + a2[i][j];
        }
    }
    return c;
}

void print(int** a, int n1, int n2)
{
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
}

void print1(int* a, int n)
{
    for (int i = 0; i < n; i++)
    {
       printf("%d ", a[i]);
      }
        printf("\n");
}

int main(int argc, char* argv[])
{
    int N = 10;
    int numtasks, rank,size;
    MPI_Status status;
    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 
    int portionB = N/size;

    int** a = (int**)malloc(N * sizeof(int*));
    int** b = (int**)malloc(N * sizeof(int*));
    int** product = (int**)malloc(N * sizeof(int*));
    int** sum = (int**)malloc(N * sizeof(int*));
    int** sub_rand_a = (int**)malloc(sizeof(int) * portionB);
    int** sub_rand_b = (int**)malloc(sizeof(int) * N);

    for (int i = 0; i < N; i++)
    {
        a[i] = (int*)malloc(N * sizeof(int));
        b[i] = (int*)malloc(N * sizeof(int));
        product[i] = (int*)malloc(N * sizeof(int));
        sum[i] = (int*)malloc(N * sizeof(int));
        sub_rand_b[i] = (int*)malloc(portionB * sizeof(int));
    }
    for (int i = 0; i < portionB; i++)
    {
        sub_rand_a[i] = (int*)malloc(N * sizeof(int));
    }
    if (rank == 0)
    {
        FILE* a10 = fopen("a10.txt", "r");
        //    if (a10 != NULL) printf("hhhh");
        FILE* b10 = fopen("b10.txt", "r");
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                fscanf(a10, "%d", &a[i][j]);
                fscanf(b10, "%d", &b[i][j]);
            }
        }
        fclose(a10);
        fclose(b10);
    }
    print(a,N,N);
    int *message = (int*)malloc(N*portionB * sizeof(int));
    int* messageb = (int*)malloc(N * portionB * sizeof(int));
    int k = 0;
    int lower = 0;
    int upper = portionB;

    //scatter

    for (int p = 1; p < size; p++)
    {
        
        //запаковка
        if (rank==0)
        {
            lower = upper;
            upper = upper + portionB;
            for (int i = lower; i < upper; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    message[k] = a[i][j];
                    //printf(" %d", message[k]);
                    k++;
                }
            }
            k = 0;
            for (int i = 0; i < N; i++)
            {
                for (int j = lower; j < upper; j++)
                {
                    messageb[k] = a[i][j];

                    k++;
                }
            }
        }
        

        //print1(messageb, N * portionB);
        //printf("\n");
        print1(message, N * portionB);
        if (rank == 0)
        {
            //MPI_Send(&message, N * portionB, MPI_INT, p, p, MPI_COMM_WORLD);
            print1(message, N * portionB);
        }
        else if (rank==p)
        {
           print1(message, N * portionB);
           //MPI_Recv(&message, N * portionB, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
           //print1(message, N * portionB);
           //распаковка
           k = 0;
           for (int i = 0; i < portionB; i++)
           {
               for (int j = 0; j < N; j++)
               {

                   sub_rand_a[i][j] = message[k];

                   k++;
               }
           }
           k = 0;
           for (int i = 0; i < N; i++)
           {
               for (int j = 0; j < portionB; j++)
               {
                   sub_rand_b[i][j] = messageb[k];
                   k++;
               }
           }
           print(sub_rand_a,portionB,N);
        }

        
        
    }
    /*
    if (rank == 0)
    {
        product = multiplication(a, b,portionB);
        //print(product,N);
        MPI_Send(&product, 1, MPI_INT, (rank + 1) % size, rank, MPI_COMM_WORLD);
        MPI_Recv(&a, 1, MPI_INT, (rank + size - 1) % size, (rank + size - 1) % (size), MPI_COMM_WORLD, &status);

    }

    else
    {
        MPI_Recv(&product, 1, MPI_INT, (rank + size - 1) % size, (rank + size - 1) % (size), MPI_COMM_WORLD, &status);
        //product = multiplication(a, b, 0, portionB);
        sum = summing(product, multiplication(a, b, portionB, N),N);
        MPI_Send(&sum, 1, MPI_INT, (rank + 1) % size, rank, MPI_COMM_WORLD);
    }
    */

    //printf("Size=%d", portionB);
    MPI_Finalize();
}