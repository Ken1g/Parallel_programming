#define JSIZE 1000
#define ISIZE 1000

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <math.h>
#include "mpi.h"


int main(int argc, char **argv)
{
    double *a = (double*) calloc(ISIZE * JSIZE, sizeof(double));
    double *b = (double*) calloc(ISIZE * JSIZE, sizeof(double));
    int i, j;
    FILE *ff;
    int ProcRank, ProcNum;

    for (i = 0; i < ISIZE; ++i) {
        for (j = 0; j < JSIZE; ++j) {
            a[i * ISIZE + j] = 10 * i + j;
            b[i * ISIZE + j] = 0; 
        }
    }

    MPI_Status Status;
    MPI_Init(&argc, &argv);
    double MPI_time = 0.0;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_time -= MPI_Wtime();
    
    int cells_in_process = JSIZE / ProcNum;
    int cells_in_last_process = JSIZE - cells_in_process * (ProcNum - 1);
    int cells_in_that_process;
    if (ProcRank == ProcNum - 1) {
        cells_in_that_process = cells_in_last_process;
    }
    else {
        cells_in_that_process = cells_in_process;
    }
    int start_j = ProcRank * cells_in_process;
    for (j = start_j; j < start_j + cells_in_that_process; ++j) {
        b[j] = a[j]; 
    }

    int cell_counter;

    for (i = 1; i < ISIZE; ++i) {        
        int current_j = start_j;
        int new_current_j = (current_j - 1 + JSIZE) % JSIZE;
        for (cell_counter = 0; cell_counter < cells_in_that_process; ++cell_counter) {
            if (current_j != 0) { 
                a[i * ISIZE + new_current_j] = sin(a[(i - 1) * ISIZE + current_j]);
            }
            b[i * ISIZE + new_current_j] = a[i * ISIZE + new_current_j];
            current_j = (current_j + 1) % JSIZE;
            new_current_j = (current_j - 1 + JSIZE) % JSIZE;
        }
        start_j = (start_j - 1 + JSIZE) % JSIZE;
    }
    
    double *result = (double*) calloc(ISIZE * JSIZE, sizeof(double));

    MPI_Reduce(
        b,
        result,
        ISIZE * JSIZE,
        MPI_DOUBLE,
        MPI_SUM,
        0,
        MPI_COMM_WORLD
    );
    MPI_time += MPI_Wtime();
    if (ProcRank == 0) {
        printf("MPI time = %.7lf\n", MPI_time); 
    }

    if (ProcRank == 0) {
        ff = fopen("1a", "w");
        for (i = 0; i < ISIZE; i++) {
            for (j = 0; j < JSIZE; j++){
                fprintf(ff,"%.3lf ", result[i * ISIZE + j]);
            }
            fprintf(ff, "\n");
        }
        fclose(ff);
    }
    free(result);
    free(a);
    free(b);
    MPI_Finalize();

    return 0;
}
