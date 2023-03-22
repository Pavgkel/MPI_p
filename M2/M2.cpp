#include<stdio.h>
#include<mpi.h>
#include <stdlib.h>
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
using namespace std;

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
            for (int inner = 0; inner < N; inner++)
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

int* add(int* a1, int* a2, int n1,int n2,int N)
{
    int* c = (int*)malloc(N * sizeof(int*));
    
    for (int i = n1; i < n2; i++)
    { 
     c[i] = a1[i]; 
    }
    
    return c;
}

int main(int argc, char* argv[])
{
    int N = 2048;
    int numtasks, rank, size;
    MPI_Status status;
    double t1 = 0, t2 = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    t1 = MPI_Wtime();
    int portionB = N / size;

    int** a = (int**)malloc(N * sizeof(int*));
    int** b = (int**)malloc(N * sizeof(int*));
    int** product = (int**)malloc(N * sizeof(int*));
    int** product0 = (int**)malloc(N * sizeof(int*));
    int** sum = (int**)malloc(N * sizeof(int*));
    int** sub_rand_a = (int**)malloc(sizeof(int) * portionB);
    int** sub_rand_b = (int**)malloc(sizeof(int) * N);

    int** sub_rand_a0 = (int**)malloc(sizeof(int) * portionB);
    int** sub_rand_b0 = (int**)malloc(sizeof(int) * N);

    for (int i = 0; i < N; i++)
    {
        a[i] = (int*)malloc(N * sizeof(int));
        b[i] = (int*)malloc(N * sizeof(int));
        product[i] = (int*)malloc(N * sizeof(int));
        product0[i] = (int*)malloc(N * sizeof(int));
        sum[i] = (int*)malloc(N * sizeof(int));
        sub_rand_b[i] = (int*)malloc(portionB * sizeof(int));
        sub_rand_b0[i] = (int*)malloc(portionB * sizeof(int));
    }
    for (int i = 0; i < portionB; i++)
    {
        sub_rand_a[i] = (int*)malloc(N * sizeof(int));
        sub_rand_a0[i] = (int*)malloc(N * sizeof(int));
    }
    if (rank == 0)
    {
        
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                a[i][j] = rand() % 666;
                b[i][j] = rand() % 666;
            }
        }
        /*
        printf("Matrix A\n");
        print(a, N, N);
        printf("\n");
        printf("Matrix B\n");
        print(b, N, N);
        printf("\n");
        */
    }
 
    int* message = (int*)malloc(N * portionB * sizeof(int));
    int* messagea1 = (int*)malloc(N * portionB * sizeof(int));
    int* message1 = (int*)malloc(N * portionB * sizeof(int));
    int* gumbold = (int*)malloc(portionB * portionB * sizeof(int));
    int* hresult = (int*)malloc(portionB * portionB*size * sizeof(int));
    int* gresult = (int*)malloc(N*N * sizeof(int));
    int* message0 = (int*)malloc(size*portionB * portionB * sizeof(int));
    int* messageb = (int*)malloc(N * portionB * sizeof(int));
    int* sendarray= (int*)malloc(N*N* sizeof(int));
    
    int k = 0;
    int lower = 0;
    int upper = portionB;
    

    //scatter
    for (int p = 1; p < size; p++)
    {

        //запаковка
        if (rank == 0)
        {
            lower = upper;
            upper = upper + portionB;
            k = 0;
            for (int i = lower; i < upper; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    message[k] = a[i][j];
                    k++;
                }
            }
            
            k = 0;
            for (int i = 0; i < N; i++)
            {
                for (int j = lower; j < upper; j++)
                {
                    messageb[k] = b[i][j];
                    k++;
                }
            }

        }
        /*
        printf("numProc=%d", rank);
        printf("\n");
        printf("Part message A\n");
        */
        //print1(messageb, N * portionB);
        //printf("\n");
        //print1(message, N * portionB);
        if (rank == 0)
        {
            MPI_Send(message, N * portionB, MPI_INT, p, p, MPI_COMM_WORLD);
            MPI_Send(messageb, N * portionB, MPI_INT, p, p, MPI_COMM_WORLD);
            //print1(message, N * portionB);
            //printf("\n");
        }
        else if (rank == p)
        {
            //print1(message, N * portionB);
            MPI_Recv(message, N * portionB, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
            MPI_Recv(messageb, N * portionB, MPI_INT, 0, rank, MPI_COMM_WORLD, &status);
            //print1(message, N * portionB);
            //printf("\n");
            /*
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
            
            printf("numProc=%d", rank);
            printf("\n");
            printf("Part A\n");
            
            print(sub_rand_a, portionB, N);
            printf("\n");
            printf("Part B\n");
            print(sub_rand_b, N, portionB);
            printf("\n");
            
            printf("numProc=%d", rank);
            printf("\n");
            printf("Multiplication\n");
            product = multiplication(sub_rand_a, sub_rand_b, portionB,N);
            print(product,portionB,portionB);
            printf("\n");
            */
            //MPI_Send(message, N* portionB, MPI_INT, (rank + size - 1) % size, (rank + size - 1) % (size), MPI_COMM_WORLD);
            //MPI_Recv(message, N* portionB, MPI_INT, (rank + 1) % size, rank, MPI_COMM_WORLD, &status);
        }
    }
    int lower0 = 0;
    int upper0 = portionB*portionB;
    int lower1 = 0;
    int upper1 = portionB * portionB * size;
    lower = 0;
    upper = portionB;
    
    //запаковыаем данные для 0 процесса
    if (rank == 0)
    {
        
        for (int i = 0; i < portionB; i++)
        {
            for (int j = 0; j < N; j++)
            {
                sub_rand_a[i][j] = a[i][j];
            }
        }
        
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < portionB; j++)
            {
                sub_rand_b[i][j] = b[i][j];
            }
        }
        //print(sub_rand_a, portionB, N);
        //printf("\n");
        //запаковываем для отправки
            
            k = 0;
            for (int i = lower; i < upper; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    //printf("numProc=%d", sub_rand_a[i][j]);
                    message[k] = sub_rand_a[i][j];
                    k++;
                }
            }
            

            k = 0;
            for (int i = 0; i < N; i++)
            {
                for (int j = lower; j < upper; j++)
                {
                    messageb[k] = sub_rand_b[i][j];
                    k++;
                }
            }
            lower = upper;
            upper = upper + portionB;
            //print1(message0, N* portionB);
            //MPI_Send(message, N* portionB, MPI_INT, 1, 0, MPI_COMM_WORLD);
            //MPI_Send(messageb, N* portionB, MPI_INT, 1, 0, MPI_COMM_WORLD);
    }
    
    /*
    else 
    {
        MPI_Recv(message, N* portionB, MPI_INT, (rank + size - 1) % size, (rank + size - 1) % (size), MPI_COMM_WORLD, &status);
        MPI_Recv(messageb, N* portionB, MPI_INT, (rank + size - 1) % size, (rank + size - 1) % (size), MPI_COMM_WORLD, &status);
    }
    */
    //k = 0;
    //распаковка и умножение по кольцу
    for (int p = 0; p < size; p++)
    {
            //запаковываем для отправки
            /*
            k = 0;
            for (int i = lower; i < upper; i++)
            {
                for (int j = 0; j < N; j++)
                {
                    printf("numProc=%d", sub_rand_a[i][j]);
                    //message[k] = sub_rand_a[i][j];
                    k++;
                }
            }
            lower = upper;
            upper = upper + portionB;
            */
            //MPI_Send(message, N* portionB, MPI_INT, (rank + 1) % size, rank, MPI_COMM_WORLD);
            //MPI_Recv(message, N* portionB, MPI_INT, (rank + size - 1) % size, (rank + size - 1) % size, MPI_COMM_WORLD, &status);
        
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
        
            
        
            //MPI_Send(message, N * portionB, MPI_INT, (rank + 1) % size, rank, MPI_COMM_WORLD);
            //MPI_Recv(message, N * portionB, MPI_INT, (rank + size - 1) % size, (rank + size - 1) % size, MPI_COMM_WORLD, &status);
            
            if (rank == 0)
            {
                
                MPI_Send(message, N * portionB, MPI_INT, (rank + 1) % size, rank, MPI_COMM_WORLD);
                //printf("Send 0\n");
                //print1(message, N* portionB);
                //MPI_Send(messageb, N* portionB, MPI_INT, (rank + 1) % size, rank, MPI_COMM_WORLD);
                MPI_Recv(message, N* portionB, MPI_INT, size-1, size-1, MPI_COMM_WORLD, &status);
                //MPI_Recv(messageb, N* portionB, MPI_INT, (rank + size - 1) % size, (rank + size - 1) % (size), MPI_COMM_WORLD, &status);
            }

            else
            {
                MPI_Recv(message, N * portionB, MPI_INT, (rank + size - 1) % size, (rank + size - 1) % (size), MPI_COMM_WORLD, &status);
                //MPI_Recv(messageb, N* portionB, MPI_INT, (rank + size - 1) % size, (rank + size - 1) % (size), MPI_COMM_WORLD, &status);
                //MPI_Recv(messageb, N * portionB, MPI_INT, (rank + size - 1) % size, (rank + size - 1) % (size), MPI_COMM_WORLD, &status);
                MPI_Send(message, N * portionB, MPI_INT, (rank + 1) % size, rank, MPI_COMM_WORLD);
                //printf("Send 1\n");
                //print1(message, N* portionB);
                //MPI_Send(messageb, N* portionB, MPI_INT, (rank + 1) % size, rank, MPI_COMM_WORLD);
                //MPI_Recv(messageb, N* portionB, MPI_INT, (rank + size - 1) % size, (rank + size - 1) % (size), MPI_COMM_WORLD, &status);
                //MPI_Send(messageb, N* portionB, MPI_INT, (rank + 1) % size, rank, MPI_COMM_WORLD);
            }
        /*
        printf("numProc=%d", rank);
        printf("\n");
        printf("Part A\n");
        //print1(message, N* portionB);
        print(sub_rand_a, portionB, N);
        printf("\n");
        printf("numProc=%d", rank);
        printf("\n");
        printf("Part B\n");
        print(sub_rand_b, N, portionB);
        //print1(messageb, N* portionB);
        printf("\n");
        
        printf("numProc=%d", rank);
        printf("\n");
        printf("Multiplication\n");
        */

        product = multiplication(sub_rand_a, sub_rand_b, portionB, N);
        //запаковываем результат
        k = 0;
        
        for (int i = 0; i < portionB; i++)
        {
            for (int j = 0; j < portionB; j++)
            {
                message0[k] = product[i][j];
                k++;
            }
        }
        //сбор всех данных с процесса
        k = 0;
        for (int i = lower0; i < upper0; i++)
        {
            hresult[i] = message0[k];
            k++;
        }
        lower0 = upper0;
        upper0 = upper0 + portionB*portionB;
        /*
        
        */
        //print1(message0, portionB* portionB);
        // 
        //print(product, portionB, portionB);
        //printf("\n");
        
    }
    
    //MPI_Gather(sendarray, portionB* portionB*size, MPI_INT, hresult, portionB* portionB*size, MPI_INT, 0, MPI_COMM_WORLD);
    t2 = MPI_Wtime();
    cout << t2 - t1;
    //print1(sendarray, N*N);
    //MPI_Recv(message, N* portionB, MPI_INT, (rank + size - 1) % size, (rank + size - 1) % (size), MPI_COMM_WORLD, &status);
    //if (rank==0) print1(message0, portionB*portionB*size);
    
    
    //message0 = 0;
    //sendrarray
    //MPI_Gather(sendrarray, portionB*portionB, MPI_INT, message0, portionB* portionB, MPI_INT, rank, MPI_COMM_WORLD);
    //print(product, portionB, portionB);
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