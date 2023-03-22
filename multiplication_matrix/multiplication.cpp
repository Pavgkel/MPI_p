#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <malloc.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <iomanip>

using namespace std;

void print(int** a, int n1, int n2)
{
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
}

int main(int* argc, char** argv)
{
    int N=2048;

	//ifstream in("matrix.txt");
	//in >> a >>b;

    MPI_Init(argc, &argv);
    double t1 = 0, t2 = 0, t3=0, t4=0;
	
    int** a1 = (int**)malloc(N * sizeof(int*));
    int** a2 = (int**)malloc(N * sizeof(int*));
  

    for (int i = 0; i < N; i++)
    {
        a1[i] = (int*)malloc(N * sizeof(int));
        a2[i] = (int*)malloc(N * sizeof(int));
      
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            a1[i][j] = rand() % 666;
            a2[i][j] = rand() % 666;

        }
    }

    
    t1 = MPI_Wtime();
    //умножение матриц
    int** product = (int**)malloc(N * sizeof(int*));
    for (int row = 0; row < N; row++) 
    {
        product[row] = (int*)malloc(N * sizeof(int));
        for (int col = 0; col < N; col++) 
        {
            product[row][col] = 0;
            // Multiply the row of A by the column of B to get the row, column of product.
            for (int inner = 0; inner < N; inner++) 
            {
                product[row][col] += a1[row][inner] * a2[inner][col];
                
            }
        }
    }
    t2 = MPI_Wtime();
    printf("Time=%g", t2 - t1);
    printf("\n");
    MPI_Finalize();
    /*
    print(a1,N,N);
    printf("\n");
    print(a2, N, N);
    printf("\n");
    print(product,N,N);
    
    // Печать 1 матрицы
    for_each(matrix1.begin(), matrix1.end(), [](std::vector<int>& ivec)
        {
            for_each(ivec.begin(), ivec.end(), [](int i)
                {
                    cout << left << setw(5) << i;
                });
            cout << endl;
        });

    // Печать 2 матрицы
    for_each(matrix2.begin(), matrix2.end(), [](std::vector<int>& ivec)
        {
            for_each(ivec.begin(), ivec.end(), [](int i)
                {
                    cout << left << setw(5) << i;
                });
            cout << endl;
        });

    */
}