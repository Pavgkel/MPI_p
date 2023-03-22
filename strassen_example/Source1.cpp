#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <vector>
#include <iostream> 

using namespace std;

#define N 2048

int S1[N / 2][N / 2];
int S2[N / 2][N / 2];
int S3[N / 2][N / 2];
int S4[N / 2][N / 2];
int S5[N / 2][N / 2];
int S6[N / 2][N / 2];
int S7[N / 2][N / 2];

int** add(int** a1, int** a2, int n)
{
	//unique_ptr<unique_ptr<int[]>[]> c(new unique_ptr<int[]>[2]());
	int** c = (int**)malloc(n * sizeof(int*));
	//vector<vector<int> > c(n);
	
		for (int i = 0; i < n; i++)
		{
			//c[i].resize(n);
			c[i] = (int*)malloc(n * sizeof(int));
			//v1[i]= unique_ptr<int[]>(new int[n]);
		}

		for (int i = 0; i < n; i++)
		{
			for (int j = 0; j < n; j++)
			{
				c[i][j] = a1[i][j] + a2[i][j];
				//v1[i][j]= a1[i][j] + a2[i][j];
			}
		}
	return c;
}

int** sub(int** a1, int** a2, int n)
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
				c[i][j] = a1[i][j] - a2[i][j];
			}
		}
		return c;
}

int** grub(int** matrix1, int** matrix2, int n)
{
	int** x = (int**)malloc(n * sizeof(int*));
	for (int row = 0; row < n; row++)
	{
		x[row] = (int*)malloc(n * sizeof(int));
		for (int col = 0; col < n; col++)
		{
			// Multiply the row of A by the column of B to get the row, column of product.
			for (int inner = 0; inner < n; inner++)
			{
				x[row][col]+= matrix1[row][inner] * matrix2[inner][col];
			}
		}
	}
	return x;
}


int** Strassen(int** a1, int** a2, int n)
{
	int** x = (int**)malloc(n * sizeof(int*));

	for (int i = 0; i < n; i++)
	{
		//x[i].resize(n);
		x[i] = (int*)malloc(n * sizeof(int));
	}
	/*
	if (n == 2 || n == 1) {
		x[0][0] = a1[0][0] * a2[0][0] + a1[0][1] * a2[1][0];
		x[0][1] = a1[0][0] * a2[0][1] + a1[0][1] * a2[1][1];
		x[1][0] = a1[1][0] * a2[0][0] + a1[1][1] * a2[1][0];
		x[1][1] = a1[1][0] * a2[0][1] + a1[1][1] * a2[1][1];
		return x;
	}
	*/
	if (n < 512)
	{
		x=grub(a1,a2,n);
		return x;
	}
	//printf("s=,%d",n);
	int** a = (int**)malloc(n / 2 * sizeof(int*));
	int** b = (int**)malloc(n / 2 * sizeof(int*));
	int** c = (int**)malloc(n / 2 * sizeof(int*));
	int** d = (int**)malloc(n / 2 * sizeof(int*));
	int** e = (int**)malloc(n / 2 * sizeof(int*));
	int** f = (int**)malloc(n / 2 * sizeof(int*));
	int** g = (int**)malloc(n / 2 * sizeof(int*));
	int** h = (int**)malloc(n / 2 * sizeof(int*));
	int** I = (int**)malloc(n / 2 * sizeof(int*));
	int** J = (int**)malloc(n / 2 * sizeof(int*));
	int** k = (int**)malloc(n / 2 * sizeof(int*));
	int** l = (int**)malloc(n / 2 * sizeof(int*));

	int** s1 = (int**)malloc(n / 2 * sizeof(int*));
	int** s2 = (int**)malloc(n / 2 * sizeof(int*));
	int** s3 = (int**)malloc(n / 2 * sizeof(int*));
	int** s4 = (int**)malloc(n / 2 * sizeof(int*));
	int** s5 = (int**)malloc(n / 2 * sizeof(int*));
	int** s6 = (int**)malloc(n / 2 * sizeof(int*));
	int** s7 = (int**)malloc(n / 2 * sizeof(int*));
	for (int i = 0; i < n / 2; i++)
	{
		a[i] = (int*)malloc(n / 2 * sizeof(int));
		b[i] = (int*)malloc(n / 2 * sizeof(int));
		c[i] = (int*)malloc(n / 2 * sizeof(int));
		d[i] = (int*)malloc(n / 2 * sizeof(int));
		e[i] = (int*)malloc(n / 2 * sizeof(int));
		f[i] = (int*)malloc(n / 2 * sizeof(int));
		g[i] = (int*)malloc(n / 2 * sizeof(int));
		h[i] = (int*)malloc(n / 2 * sizeof(int));
		I[i] = (int*)malloc(n / 2 * sizeof(int));
		J[i] = (int*)malloc(n / 2 * sizeof(int));
		k[i] = (int*)malloc(n / 2 * sizeof(int));
		l[i] = (int*)malloc(n / 2 * sizeof(int));
		s1[i] = (int*)malloc(n / 2 * sizeof(int));
		s2[i] = (int*)malloc(n / 2 * sizeof(int));
		s3[i] = (int*)malloc(n / 2 * sizeof(int));
		s4[i] = (int*)malloc(n / 2 * sizeof(int));
		s5[i] = (int*)malloc(n / 2 * sizeof(int));
		s6[i] = (int*)malloc(n / 2 * sizeof(int));
		s7[i] = (int*)malloc(n / 2 * sizeof(int));
	}

	for (int i = 0; i < n / 2; i++) {
		for (int j = 0; j < n / 2; j++) {
			a[i][j] = a1[i][j];
			b[i][j] = a1[i][j + n / 2];
			c[i][j] = a1[i + n / 2][j];
			d[i][j] = a1[i + n / 2][j + n / 2];
			e[i][j] = a2[i][j];
			f[i][j] = a2[i][j + n / 2];
			g[i][j] = a2[i + n / 2][j];
			h[i][j] = a2[i + n / 2][j + n / 2];
		}
	}
	
	
	s1 = Strassen(a, sub(f, h, n / 2), n / 2);
	s2 = Strassen(add(a, b, n / 2), h, n / 2);
	s3 = Strassen(add(c, d, n / 2), e, n / 2);
	s4 = Strassen(d, sub(g, e, n / 2), n / 2);
	s5 = Strassen(add(a, d, n / 2), add(e, h, n / 2), n / 2);
	s6 = Strassen(sub(b, d, n / 2), add(g, h, n / 2), n / 2);
	s7 = Strassen(sub(a, c, n / 2), add(e, f, n / 2), n / 2);


	I = add(s5, add(s6, sub(s4, s2, n / 2), n / 2), n / 2);
	J = add(s1, s2, n / 2);
	k = add(s3, s4, n / 2);
	l = add(sub(sub(s1, s7, n / 2), s3, n / 2), s5, n / 2);
	for (int i = 0; i < n / 2; i++)
	{
		for (int j = 0; j < n / 2; j++)
		{
			x[i][j] = I[i][j];
			x[i][j + n / 2] = J[i][j];
			x[i + n / 2][j] = k[i][j];
			x[i + n / 2][j + n / 2] = l[i][j];
		}
	}

	if (n == N) {
		for (int i = 0; i < n / 2; i++) {
			for (int j = 0; j < n / 2; j++) {
				S1[i][j] = s1[i][j];
				S2[i][j] = s2[i][j];
				S3[i][j] = s3[i][j];
				S4[i][j] = s4[i][j];
				S5[i][j] = s5[i][j];
				S6[i][j] = s6[i][j];
				S7[i][j] = s7[i][j];
			}
		}
	}
	return x;
	
	free(s1);
	free(s2);
	free(s3);
	free(s4);
	free(s5);
	free(s6);
	free(s7);
	free(a);
	free(b);
	free(c);
	free(d);
	free(e);
	free(f);
	free(g);
	free(h);
	free(I);
	free(J);
	free(k);
	free(l);
	
}


void print(int a[N / 2][N / 2], int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
}

int main(int* argc, char** argv)
{
	double t1 = 0, t2 = 0;

	int** a1 = (int**)malloc(N * sizeof(int*));
	int** a2 = (int**)malloc(N * sizeof(int*));
	//vector<vector<int> > a1(N);
	//vector<vector<int> > a2(N);

	for (int i = 0; i < N; i++)
	{
		a1[i] = (int*)malloc(N * sizeof(int));
		a2[i] = (int*)malloc(N * sizeof(int));
		//a1[i].resize(N);
		//a2[i].resize(N);
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
		{
			a1[i][j] = rand() % 666;
			a2[i][j] = rand() % 666;

		}
	}
	MPI_Init(argc, &argv);
	t1 = MPI_Wtime();
	//asum(a1,a2,N);
	Strassen(a1, a2, N);
	
	t2 = MPI_Wtime();
	printf("Time=%g", t2 - t1);
	/*
	print(S1, N / 2);
	print(S2, N / 2);
	print(S3, N / 2);
	print(S4, N / 2);
	print(S5, N / 2);
	print(S6, N / 2);
	print(S7, N / 2);
	*/
	MPI_Finalize();
}


