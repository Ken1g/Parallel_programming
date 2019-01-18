#define JSIZE 10
#define ISIZE 10
#define ITER 1000

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
    double *receiving = (double*) calloc(JSIZE, sizeof(double));
    int i, j;
    FILE *ff;
    int ProcRank, ProcNum, RecvRank;


    for (i = 0; i < ISIZE; ++i) {
        for (j = 0; j < JSIZE; ++j) {
            a[i * ISIZE + j] = 0;
            b[i * ISIZE + j] = 0; 
        }
    }
    for (j = 0; j < JSIZE; ++j) {
        a[j] = 200;
        a[(ISIZE - 1) * ISIZE + j] = 400;
        b[j] = 200;
        b[(ISIZE - 1) * ISIZE + j] = 400;
    }
    for (i = 0; i < ISIZE; ++i) {
        a[i * JSIZE] = 100;
        a[i * JSIZE + JSIZE - 1] = 300;
        b[i * JSIZE] = 100;
        b[i * JSIZE + JSIZE - 1] = 300;
    }

    ff = fopen("YA", "w");
    fclose(ff);

    MPI_Status Status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

    int rows_number = ISIZE - 2;
    int rows_in_process = rows_number / ProcNum;
    int rows_in_last_process = rows_number - (rows_in_process * (ProcNum - 1));
    int rows_in_that_process = rows_in_process;

    if (ProcRank == ProcNum - 1) {
        rows_in_that_process = rows_in_last_process;
    }
    int starting_row = ProcRank * rows_in_process + 1;
    int iteration;

    for (iteration = 0; iteration < ITER; ++iteration) {
        for (i = starting_row; i < starting_row + rows_in_that_process; ++i) {
            for (j = 1; j < JSIZE - 1; ++j) {
                b[i * ISIZE + j] = (
                    a[i * ISIZE + j - 1] + 
                    a[i * ISIZE + j + 1] + 
                    a[(i - 1) * ISIZE + j] + 
                    a[(i + 1) * ISIZE + j]
                ) / 4;
            }
        }
        int last_row = starting_row + rows_in_that_process - 1;
        //printf("%d %d %d\n", starting_row, last_row, rows_in_that_process);

    // Sending and Recieving
        if (ProcRank == 0) {
            MPI_Send(
                &(b[last_row * JSIZE]),
                JSIZE,
                MPI_DOUBLE,
                ProcRank + 1,
                0,
                MPI_COMM_WORLD
            );
        }
        else {
            MPI_Recv(
                &(b[(starting_row - 1) * JSIZE]),
                JSIZE,
                MPI_DOUBLE,
                ProcRank - 1,
                0,
                MPI_COMM_WORLD,
                &Status
            );
            if (ProcRank != (ProcNum - 1)) {
                MPI_Send(
                    &(b[(last_row * JSIZE)]),
                    JSIZE,
                    MPI_DOUBLE,
                    ProcRank + 1,
                    0,
                    MPI_COMM_WORLD
                );
            }
        }

        MPI_Barrier(MPI_COMM_WORLD);

        if (ProcRank == ProcNum - 1) {
            MPI_Send(
                &(b[starting_row * JSIZE]),
                JSIZE,
                MPI_DOUBLE,
                ProcRank - 1,
                0,
                MPI_COMM_WORLD
            );
        }
        else {
            MPI_Recv(
                &(b[(last_row + 1) * JSIZE]),
                JSIZE,
                MPI_DOUBLE,
                ProcRank + 1,
                0,
                MPI_COMM_WORLD,
                &Status
            );
            if (ProcRank != 0) {
                MPI_Send(
                    &(b[(starting_row * JSIZE)]),
                    JSIZE,
                    MPI_DOUBLE,
                    ProcRank - 1,
                    0,
                    MPI_COMM_WORLD
                );
            }
        }

    // Print in File and updating a
        if (ProcRank == 0) {
            ff = fopen("YA", "a");
            for (i = starting_row - 1; i <= last_row; i++) {
                for (j = 0; j < JSIZE; j++){
                    fprintf(ff,"%.2f ", b[i * ISIZE + j]);
                }
                fprintf(ff, "\n");
            }
            fclose(ff);
            MPI_Send(
                NULL,
                0,
                MPI_INT,
                ProcRank + 1,
                0,
                MPI_COMM_WORLD
            );
        }
        else {
            MPI_Recv(
                NULL,
                0,
                MPI_INT,
                ProcRank - 1,
                0,
                MPI_COMM_WORLD,
                &Status
            );

            ff = fopen("YA", "a");
            for (i = starting_row; i <= last_row; i++) {
                for (j = 0; j < JSIZE; j++){
                    fprintf(ff,"%.2f ", b[i * ISIZE + j]);
                }
                fprintf(ff, "\n");
            }
            fclose(ff);

            if (ProcRank != (ProcNum - 1)) {
                MPI_Send(
                    NULL,
                    0,
                    MPI_INT,
                    ProcRank + 1,
                    0,
                    MPI_COMM_WORLD
                );
            }
            else {
                ff = fopen("YA", "a");
                for (j = 0; j < JSIZE; j++){
                    fprintf(ff,"%.2f ", b[(ISIZE - 1) * ISIZE + j]);
                }
                fprintf(ff, "\n\n");
                fclose(ff);
            }
        }

    // Update a[]
        for (i = 0; i < ISIZE; i++) {
            for (j = 0; j < JSIZE; j++){
                a[i * JSIZE + j] = b[i * ISIZE + j];
            }
        }
    }
    
/*    
    if (ProcRank == 0) {
        ff = fopen("YA", "w");
        for (i = 0; i < ISIZE; i++) {
            for (j = 0; j < JSIZE; j++){
                fprintf(ff,"%.2f ", b[i * ISIZE + j]);
            }
            fprintf(ff, "\n");
        }
        fclose(ff);
    }
*/

    free(a);
    free(b);
    MPI_Finalize();
}
