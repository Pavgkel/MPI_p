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

int main(int* argc, char** argv)
{
    int a=500, b=500;

    MPI_Init(argc, &argv);
    double t1 = 0, t2 = 0, t3 = 0, t4 = 0;
   

    vector<vector<int> > matrix1(b);
    vector<vector<int> > matrix2(a);

    // Заполнение 1 матрицы
    for (size_t i = 0; i < b; ++i)
    {
        matrix1[i].resize(a);
        for (size_t j = 0; j < a; ++j)
            //in >> matrix1[i][j];
            matrix1[i][j] = rand() % 666;
    }
    // Заполнение 2 матрицы
    for (size_t i = 0; i < a; ++i)
    {
        matrix2[i].resize(b);
        for (size_t j = 0; j < b; ++j)
            //in >> matrix2[i][j];
            matrix2[i][j] = rand() % 666;
    }

    vector<vector<int> > product(b);
    t1 = MPI_Wtime();
    //умножение матриц
    for (int row = 0; row < b; row++)
    {
        product[row].resize(b);
        for (int col = 0; col < b; col++)
        {
            // Multiply the row of A by the column of B to get the row, column of product.
            for (int inner = 0; inner < a; inner++)
            {

                product[row][col] += matrix1[row][inner] * matrix2[inner][col];

            }
            //cout << product[row][col] << "  ";
        }
        //cout << "\n";
    }
    t2 = MPI_Wtime();
    printf("Time=%g", t2 - t1);
    MPI_Finalize();
}
