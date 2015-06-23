
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <iostream>
#include <stdlib.h>

#define N 4096
#define T 1024

using namespace std;

__global__
void suma_vectores(int* a, int* b, int* c)
{
	int i = blockIdx.x * blockDim.x + threadIdx.x;
	if (i < N)
		c[i] = a[i] + b[i];
}

void llenar_vector(int* a)
{
	int i;
	for(i = 0; i < N; i++)
		a[i] = rand()%T;
}

void print_vector(int* a)
{
	int i;
	for(i = 0; i < N; i++)
		cout<<a[i]<<" ";
	cout<<endl;
}

int main()
{
	int a[N], b[N], c[N];
	llenar_vector(a);
	llenar_vector(b);
	int *d_a, *d_b, *d_c;	int size = N * sizeof(int);
	int tmp = ceil(N/T);

	cudaMalloc((void**)&d_a, size);
	cudaMalloc((void**)&d_b, size);
	cudaMalloc((void**)&d_c, size);

	cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

	suma_vectores<<<tmp, T>>>(d_a, d_b, d_c);

	cudaMemcpy(c, d_c, size,cudaMemcpyDeviceToHost);

	print_vector(c);

	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);

    return 0;
}