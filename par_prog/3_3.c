#define OK					 100
#define FILE_READ_ERROR 	-100
#define INCORRECT_INPUT 	-101
#define NOT_A_NUMBER       	-102

#define MAX_LENGTH_OF_STR 	 1024

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

int main(int argc, char** argv) {
	FILE *input, *output;
	int i, rank, size;
	char *first, *second;

	MPI_Status Status;
	double full_time = 0.0;

	first = (char*) calloc(MAX_LENGTH_OF_STR / 2 + 2, sizeof(char));
	second = (char*) calloc(MAX_LENGTH_OF_STR / 2 + 2, sizeof(char));

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Barrier(MPI_COMM_WORLD); // start of timer
	full_time -= MPI_Wtime();

	if (!rank) {
		input = fopen("input.txt", "rb");
		output = fopen("output.txt", "wba");
		if (argc != 1) {
    		printf("INCORRECT_INPUT\n");
        	return INCORRECT_INPUT;
    	}
		if (input == NULL) {
			printf("FILE_READ_ERROR\n");
			return(FILE_READ_ERROR);
		}
		else {
			char d;
			first[0] = fscanf(input, " %d  ", &d);
			printf("%c", d);
		}
	}

	//printf("%d %d", first[0], second[0]);

	int size_in_each_process = MAX_LENGTH_OF_STR / size;

	MPI_Finalize();
	return 0;
}