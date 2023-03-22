#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <vector>
#include <iostream> 

using namespace std;

#define N 512

int S1[N / 2][N / 2];
int S2[N / 2][N / 2];
int S3[N / 2][N / 2];
int S4[N / 2][N / 2];
int S5[N / 2][N / 2];
int S6[N / 2][N / 2];
int S7[N / 2][N / 2];

vector<vector<int>> add(vector<vector<int>> a1, vector<vector<int>> a2, int n)
{
	vector<vector<int> > c(n);

	for (int i = 0; i < n; i++)
	{
		c[i].resize(n);
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			c[i][j] = a1[i][j] + a2[i][j];
		}
	}
	return c;
}

vector<vector<int>> sub(vector<vector<int>> a1, vector<vector<int>> a2, int n)
{
	vector<vector<int> > c(n);

	for (int i = 0; i < n; i++)
	{
		c[i].resize(n);
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
		{
			c[i][j] = a1[i][j] - a2[i][j];
		}
	}
	return c;
}



vector<vector<int>> Strassen(vector<vector<int>> a1, vector<vector<int>> a2, int n)
{
	int qqq = n / 2;
	//printf("s=,%d",n);
	vector<vector<int> > x(n);

	for (int i = 0; i < n; i++)
	{
		x[i].resize(n);
	}
	if (n == 2) {
		x[0][0] = a1[0][0] * a2[0][0] + a1[0][1] * a2[1][0];
		x[0][1] = a1[0][0] * a2[0][1] + a1[0][1] * a2[1][1];
		x[1][0] = a1[1][0] * a2[0][0] + a1[1][1] * a2[1][0];
		x[1][1] = a1[1][0] * a2[0][1] + a1[1][1] * a2[1][1];
		return x;
	}

	
	vector<vector<int> > a(n / 2);
	vector<vector<int> > b(n / 2);
	vector<vector<int> > c(n / 2);
	vector<vector<int> > d(n / 2);
	vector<vector<int> > e(n / 2);
	vector<vector<int> > f(n / 2);
	vector<vector<int> > g(n / 2);
	vector<vector<int> > h(n / 2);
	vector<vector<int> > I(n / 2);
	vector<vector<int> > J(n / 2);
	vector<vector<int> > k(n / 2);
	vector<vector<int> > l(n / 2);
	vector<vector<int> > s1(n / 2);
	vector<vector<int> > s2(n / 2);
	vector<vector<int> > s3(n / 2);
	vector<vector<int> > s4(n / 2);
	vector<vector<int> > s5(n / 2);
	vector<vector<int> > s6(n / 2);
	vector<vector<int> > s7(n / 2);
	

	
	for (int i = 0; i < n / 2; i++)
	{
		a[i].resize(n / 2);
		b[i].resize(n / 2);
		c[i].resize(n / 2);
		d[i].resize(n / 2);
		e[i].resize(n / 2);
		f[i].resize(n / 2);
		g[i].resize(n / 2);
		h[i].resize(n / 2);
		I[i].resize(n / 2);
		J[i].resize(n / 2);
		k[i].resize(n / 2);
		l[i].resize(n / 2);
		s1[i].resize(n / 2);
		s2[i].resize(n / 2);
		s3[i].resize(n / 2);
		s4[i].resize(n / 2);
		s5[i].resize(n / 2);
		s6[i].resize(n / 2);
		s7[i].resize(n / 2);
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
	//vector<vector<int> >().swap(x);

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

	vector<vector<int> > a1(N);
	vector<vector<int> > a2(N);

	for (int i = 0; i < N; i++)
	{
		a1[i].resize(N);
		a2[i].resize(N);
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
	Strassen(a1, a2, N);

	t2 = MPI_Wtime();
	printf("Time=%g", t2 - t1);
	//print(S1, N / 2);
	//print(S2, N / 2);
	//print(S3, N / 2);
	//print(S4, N / 2);
	//print(S5, N / 2);
	//print(S6, N / 2);
	//print(S7, N / 2);
	MPI_Finalize();
}