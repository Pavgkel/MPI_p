#include<stdio.h>
//#include<mpi.h>
#include <stdlib.h>
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
using namespace std;

void print1(int* a, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void print(int** a, int n1,int n2)
{
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < n2; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
}

int** multiplication(int** a, int** b, int n1,int N)
{
    int** product = (int**)malloc(N * sizeof(int*));
    for (int row = 0; row < n1; row++)
    {
        product[row] = (int*)malloc(N * sizeof(int));
        for (int col = 0; col < n1; col++)
        {
            product[row][col] = 0;
            // Multiply the row of A by the column of B to get the row, column of product.
            //product[row][col] += a[row][0] * b[row][col];

            for (int inner = 0; inner < N; inner++)
            {
                product[row][col] += a[row][inner] * b[inner][col];
                //printf("%d ", product[row][col]);
            }
            //printf("\n");
            //c[j][i] = product;
        }
    }
    return product;
}

int main(int argc, char* argv[])
{
    int N = 24;
    int size = 2;
    int portionB = N / size;
    int** a = (int**)malloc(N * sizeof(int*));
    int** b = (int**)malloc(N * sizeof(int*));
    int** sub_rand_a = (int**)malloc(sizeof(int) * portionB);
    int** sub_rand_b = (int**)malloc(sizeof(int) * N);
    int** sub_rand_a0 = (int**)malloc(sizeof(int) * portionB);
    int** sub_rand_b0 = (int**)malloc(sizeof(int) * N);
    int** product = (int**)malloc(N * sizeof(int*));
    int** product0 = (int**)malloc(N * sizeof(int*));

    for (int i = 0; i < N; i++)
    {
        a[i] = (int*)malloc(N * sizeof(int));
        b[i] = (int*)malloc(N * sizeof(int));
        sub_rand_b[i] = (int*)malloc(portionB * sizeof(int));
        sub_rand_b0[i] = (int*)malloc(portionB * sizeof(int));
        product[i] = (int*)malloc(N * sizeof(int));
        product0[i] = (int*)malloc(N * sizeof(int));
    }

    for (int i = 0; i < portionB; i++)
    {
        sub_rand_a[i] = (int*)malloc(N * sizeof(int));
        sub_rand_a0[i] = (int*)malloc(N * sizeof(int));
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            a[i][j] = rand() % 666;
            b[i][j] = rand() % 666;
        }
    }
    printf("Matrix A\n");
    print(a, N,N);
    printf("\n");

    int* message = (int*)malloc(N * portionB * sizeof(int));
    int* messageb = (int*)malloc(N * portionB * sizeof(int));

    int lower0 = 0;
    int upper0 = portionB;
    int lower1 = 0;
    int upper1 = portionB;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < portionB; j++)
        {

            //upper1++;
            sub_rand_b0[i][j] = b[i][j];
            //++;
        }
    }
    for (int p = 0; p < size; p++)
    {
       
        //вычисление первого кусочка
        for (int i = 0; i < portionB; i++)
        {
            for (int j = 0; j < N; j++)
            {
                
                //upper0++;
                sub_rand_a0[i][j] = a[lower0][j];
            }
            lower0++;
        }
        lower0 = upper0;
        upper0 = upper0 + portionB;
        lower1 = upper1;
        upper1 = upper1 + portionB;
        
        //print(sub_rand_a0,N,N);
        product0 = multiplication(sub_rand_a0, sub_rand_b0, portionB, N);
        printf("Multiplication0\n");
        print(product0, portionB, portionB);
        printf("\n");
    }

    int k = 0;
    int lower = 0;
    int upper = portionB;
    
    for (int p = 1; p < size ; p++)
    {
        lower = upper;
        upper = upper + portionB;
        k = 0;
        //запаковка
        //printf("Message A\n");
        for (int i = lower; i < upper; i++)
        {
            for (int j = 0; j < N; j++)
            {
                message[k] = a[i][j];
                //printf(" %d", message[k]);
                //printf("\n");
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
        printf("\n");
        printf("Part A\n");
        //print1(message, N * portionB);
        printf("\n");
        
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
        for (int i = 0; i <N; i++)
        {
            for (int j = 0; j < portionB; j++)
            {
                sub_rand_b[i][j] = messageb[k];
                k++;
            }
        }
        
        
        print(sub_rand_a, portionB,N);
        printf("\n");
        printf("Part B\n");
        printf("\n");
        print(sub_rand_b, N, portionB);
        printf("\n");
        product = multiplication(sub_rand_a, sub_rand_b, portionB,N);
        print(product, portionB, portionB);
        printf("\n");
    }
}