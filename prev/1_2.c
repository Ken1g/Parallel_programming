#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "mpi.h"

int main(int argc, char** argv)
{
	int ProcNum, ProcRank, RecvRank, i;
	MPI_Status Status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	
	if (!ProcRank)
	{
		printf("%d\n", ProcRank);
		if (ProcNum > 1)
		{
			for (i = 1; i < ProcNum; i++)
			{
				MPI_Send(&ProcRank, 0, MPI_INT, i, 0, MPI_COMM_WORLD);		
				MPI_Recv(&RecvRank, 0, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
			}	
		}
	}
	else
	{
		
		MPI_Recv(&RecvRank, 0, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
		printf("%d\n", ProcRank);
		MPI_Send(&ProcRank, 0, MPI_INT, 0, 0, MPI_COMM_WORLD);
		
	}

	MPI_Finalize();
	return 0;
}
