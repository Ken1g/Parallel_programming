#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#define ISIZE 100
#define JSIZE 100
double a[ISIZE][JSIZE], b[ISIZE][JSIZE];

int main(int argc, char **argv)
{
	int i, j;
	FILE *ff;
	double t1,t2;
	t1 = MPI_Wtime();	
	for (i = 0; i < ISIZE; i++)
	{
		for (j = 0; j < JSIZE; j++)
		{
			a[i][j] = 10 * i + j;
			b[i][j] = 0.;
		}
	}
	for (i = 0; i < ISIZE; i++)
	{
		for (j = 0; j < JSIZE; j++)
		{
			a[i][j] = sin(0.00001 * a[i][j]); 
		}
	}
	ff = fopen("1","w");
	for(i = 0; i < ISIZE; i++)
	{
		for (j = 0; j < JSIZE; j++)
		{
			fprintf(ff,"%f ",a[i][j]);
		}
        fprintf(ff, "\n");
	}
	fclose(ff);
	t2 = MPI_Wtime();
	printf("time: %lf\n",t2 - t1);

    return 0;
}
