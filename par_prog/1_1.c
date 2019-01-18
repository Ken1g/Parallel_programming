#define MAXLEN 2

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
	int ProcNum, ProcRank, RecvRank;
	MPI_Status Status;
	char buf[MAXLEN];
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	
	if (!ProcRank)
	{
		printf("%d\n", ProcRank);
		if (ProcNum > 1)
			MPI_Send(&ProcRank, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);			
	}
	else
	{
		MPI_Recv(&RecvRank, 1, MPI_INT, ProcRank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
		printf("%d\n", RecvRank);
		if (ProcRank != ProcNum - 1)
			MPI_Send(&ProcRank, 1, MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
	return 0;
}
