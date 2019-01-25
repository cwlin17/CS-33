#include "func.h"
#include "util.h"

void func0(double *weights, double *arrayX, double *arrayY, int xr, int yr, int n)
{
	int i;
	omp_set_num_threads(30);
	#pragma omp parallel for private(i)
	for(i = 0; i < n; i++){
		weights[i] = 1/((double)(n));
		arrayX[i] = xr;
		arrayY[i] = yr;
	}
}

void func1(int *seed, int *array, double *arrayX, double *arrayY,
			double *probability, double *objxy, int *index,
			int Ones, int iter, int X, int Y, int Z, int n)
{
	int i, j;
   	int index_X, index_Y;
	int max_size = X*Y*Z;

	omp_set_num_threads(30);
	#pragma omp parallel for private(i)
   	for(i = 0; i < n; i++){
   		arrayX[i] += 1 + 5*rand2(seed, i);
   		arrayY[i] += -2 + 2*rand2(seed, i);
   	}

	omp_set_num_threads(30);
	#pragma omp parallel for private(index_X, index_Y, i, j)
   	for(i = 0; i<n; i++){
		int temp = i * Ones;
		int roundResultX = round(arrayX[i]);
		int roundResultY = round(arrayY[i]);
   		for(j = 0; j < Ones; j++){
			int temp2 = j * 2;
			int temp3 = temp + j;
   			index_X = roundResultX + objxy[temp2 + 1];
   			index_Y = roundResultY + objxy[temp2];
   			index[temp3] = fabs(index_X*Y*Z + index_Y*Z + iter);
   			if(index[temp3] >= max_size)
   				index[temp3] = 0;
   		}
   		probability[i] = 0;

   		for(j = 0; j < Ones; j++) {
			int temp4 = temp + j;
   			probability[i] += (pow((array[index[temp4]] - 100),2) -
   							  pow((array[index[temp4]]-228),2))/50.0;
   		}
   		probability[i] = probability[i]/((double) Ones);
   	}
}

void func2(double *weights, double *probability, int n)
{
	int i;
	double sumWeights=0;

	omp_set_num_threads(30);
	#pragma omp parallel for private(i)
	for(i = 0; i < n; i++) {
   		weights[i] = weights[i] * exp(probability[i]);
	}

	omp_set_num_threads(30);
	#pragma omp parallel for private (i) reduction(+:sumWeights)
   	for(i = 0; i < n; i++)
   		sumWeights += weights[i];

	omp_set_num_threads(30);
	#pragma omp parallel for private(i)
	for(i = 0; i < n; i++)
   		weights[i] = weights[i]/sumWeights;
}

void func3(double *arrayX, double *arrayY, double *weights, double *x_e, double *y_e, int n)
{
	double estimate_x=0.0;
	double estimate_y=0.0;
	int i;

	omp_set_num_threads(30);
	#pragma omp parallel for private(i) reduction(+:estimate_x, estimate_y)
	for(i = 0; i < n; i++){
   		estimate_x += arrayX[i] * weights[i];
   		estimate_y += arrayY[i] * weights[i];
   	}

	*x_e = estimate_x;
	*y_e = estimate_y;

}

void func4(double *u, double u1, int n)
{
	int i;

	omp_set_num_threads(30);
	#pragma omp parallel for private (i)
	for(i = 0; i < n; i++){
   		u[i] = u1 + i/((double)(n));
   	}
}

void func5(double *x_j, double *y_j, double *arrayX, double *arrayY, double *weights, double *cfd, double *u, int n)
{
	int i, j;
	int temp = n - 1;

	omp_set_num_threads(30);
	#pragma omp parallel for private (i, j)
	for(j = 0; j < n; j++){
   		//i = findIndex(cfd, n, u[j]);
   		i = findIndexBin(cfd, 0, n, u[j]);
   		if(i == -1)
   			i = temp;
   		x_j[j] = arrayX[i];
   		y_j[j] = arrayY[i];

   	}

	omp_set_num_threads(30);
	#pragma omp parallel for private(i)
	for(i = 0; i < n; i++){
		arrayX[i] = x_j[i];
		arrayY[i] = y_j[i];
		weights[i] = 1/((double)(n));
	}
}
