#define JSIZE 10
#define ISIZE 10

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <malloc.h>
#include <math.h>
#include "mpi.h"

int main(int argc, char **argv)
{
    double a[ISIZE * JSIZE];
    int i, j;
    FILE *ff;
    int ProcRank, ProcNum;

    for (i = 0; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
            a[i * ISIZE + j] = 10 * i + j;
        }
    }

    MPI_Status Status;
    double full_time = 0.0;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int rows_in_process = (ISIZE - 2) / ProcNum;
    int starting_row = ProcRank * rows_in_process + 1;
    if (ProcRank == ProcNum - 1) {
        rows_in_process = (ISIZE - 2) - rows_in_process * (ProcNum - 1);
    }

    for (i = starting_row; i < starting_row + rows_in_process; i++) {
        for (j = 6; j < JSIZE - 1; j++) {
            a[i * ISIZE + j] = sin(0.00001 * a[(i + 1) * ISIZE + (j - 6)]);
        }
    }

    int *recv_counts = (int*) calloc(ProcNum, sizeof(int));
    int *displs = (int*) calloc(ProcNum, sizeof(int));
    displs[0] = ISIZE;
    for (i = 0; i < ProcNum - 1; ++i) {
        recv_counts[i] = ((ISIZE - 2) / ProcNum) * ISIZE;
        displs[i + 1] = (i + 1) * recv_counts[0] + ISIZE;
    }
    recv_counts[ProcNum - 1] = ((ISIZE - 2) - (recv_counts[0] / ISIZE) * (ProcNum - 1)) * ISIZE;
    
    MPI_Gatherv(
        a + displs[ProcRank],
        recv_counts[ProcRank],
        MPI_DOUBLE,
        a,
        recv_counts,
        displs,
        MPI_DOUBLE,
        0,
        MPI_COMM_WORLD
    );

    if (ProcRank == 0) {
        ff = fopen("1d","w");
        for (i = 0; i < ISIZE; i++) {
            for (j = 0; j < JSIZE; j++){
                fprintf(ff,"%f ", a[i * ISIZE + j]);
            }
            fprintf(ff, "\n");
        }
        fclose(ff);
    }
    free(displs);
    free(recv_counts);
    MPI_Finalize();
}
