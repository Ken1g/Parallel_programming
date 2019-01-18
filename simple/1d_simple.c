#define JSIZE 100
#define ISIZE 100

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
    double a[ISIZE][JSIZE];
    int i, j;
    FILE *ff;
    int ProcRank, ProcNum;

    for (i = 0; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++) {
            a[i][j] = 10 * i + j;
        }
    }

    MPI_Status Status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int rows_in_process = ISIZE / ProcNum;
    int starting_row = ProcRank * rows_in_process;
    if (ProcRank == ProcNum - 1) {
        rows_in_process = ISIZE - rows_in_process * (ProcNum - 1);
    }
    printf("%d %d\n", ProcRank, rows_in_process);

    for (i = 1; i < ISIZE - 1; i++) {
        for (j = 6; j < JSIZE - 1; j++) {
            a[i][j] = sin(0.00001 * a[i + 1][j - 6]);
        }
    }

    MPI_Finalize();

    ff = fopen("result.txt","w");
    for (i = 0; i < ISIZE; i++) {
        for (j = 0; j < JSIZE; j++){
            fprintf(ff,"%f ", a[i][j]);
        }
        fprintf(ff, "\n");
    }
    fclose(ff);
}
