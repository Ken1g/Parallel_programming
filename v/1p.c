#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#define ISIZE 100
#define JSIZE 100
double a[ISIZE][JSIZE],b[ISIZE][JSIZE];;

int main(int argc, char **argv)
{
	double t1,t2;
	int i, j, rank, size,k;

	FILE *ff;
	MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    t1 = MPI_Wtime();

    for (i = 0;i<ISIZE/size;i++)
    {
		for(j = 0;j<JSIZE;j++)
		{
			a[i][j] = 10 * (i + ISIZE / size * rank) + j;
		}
	}
	for (i=0; i<ISIZE/size; i++)
	{
		for (j = 0; j < JSIZE; j++)
		{
			a[i][j] = sin(0.00001 * a[i][j]); 
		}
	}
	if(rank) 
	{
		for(k = 0; k < ISIZE/size;k++)
		{
			MPI_Send(a[k],JSIZE,MPI_DOUBLE,0,0,MPI_COMM_WORLD);
			//printf("send rank %d \n",rank);
		}	
	}
	else
	{
		for(i = 1;i < size;i++)
		{
			for(k = 0; k < ISIZE/size;k++)
			{
				//printf("recv rank %d \n",rank);
				MPI_Recv(a[i*ISIZE/size+k],JSIZE,MPI_DOUBLE,i,0,MPI_COMM_WORLD,NULL);
			}
		}
	}
	MPI_Barrier(MPI_COMM_WORLD);
	if(!rank)
	{
		ff = fopen("2","w");
		for(i=0; i < ISIZE; i++)
		{
			for (j=0; j < JSIZE; j++)
			{
				fprintf(ff,"%f ",a[i][j]);
			}
			fprintf(ff,"\n");
		}
		fclose(ff);
		t2 = MPI_Wtime();
		printf("time: %lf\n",t2 - t1);
    }

	MPI_Finalize();
	return 0;
}
