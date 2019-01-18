#define OK					 100
#define FILE_READ_ERROR 	-100
#define INCORRECT_INPUT 	-101
#define NOT_A_NUMBER       	-102

#define MAX_LENGTH_OF_STR 	 8

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

int into_str(char* string, int* number, int size, int rank){
	if (!rank) 
		sprintf(string, "%d", *number);		
	else {
		char *end_of_string = (char*) calloc(size + 1, sizeof(char));
		sprintf(end_of_string, "%d", *number);
		int cur_len = strlen(end_of_string);
		int n_zeros = size - cur_len;
		for (int i = 0; i < n_zeros; i ++)
			strcat(string, "0");
		strcat(string, end_of_string);
		free(end_of_string);
		}
	}
	
int into_int(char* string, int* number) {
	char* endptr;

	*number = strtol(string, &endptr, 0);
	
	if ((errno == ERANGE && (*number == LONG_MAX || *number == LONG_MIN)) || (errno != 0 && *number == 0)) {
    	printf("INCORRECT_INPUT\n");
        return INCORRECT_INPUT;
	}
    if (*endptr) {
		printf("NOT_A_NUMBER\n");
		return NOT_A_NUMBER;
	}

	return OK;
}

void subjective_sum(int* first, int* second, int size, int* transfer_not_trans, int* transfer_trans, int* sum_not_trans, int* sum_trans, int max) 
{
	*sum_not_trans = *first + *second;
	*sum_trans = *sum_not_trans + 1;
	*transfer_not_trans = 0;
	*transfer_trans = 0;

	if (*sum_not_trans >= max) {
		*transfer_not_trans = 1;
		*sum_not_trans %= max;
	}

	if (*sum_trans >= max) {
		*transfer_trans = 1;
		*sum_trans %= max;
	}
}

int main(int argc, char** argv) {
	int i, rank, size;
	FILE* input = fopen("input.txt", "r");
	FILE* output = fopen("output.txt", "wba");
	char *first, *second, *first_sub, *second_sub;
	MPI_Status Status;
	double full_time = 0.0;

	first = (char*) calloc(MAX_LENGTH_OF_STR + 1, sizeof(char));
	second = (char*) calloc(MAX_LENGTH_OF_STR + 1, sizeof(char));

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Barrier(MPI_COMM_WORLD); // start of timer
	full_time -= MPI_Wtime();

	if (argc != 1) {
    	printf("INCORRECT_INPUT\n");
        return INCORRECT_INPUT;
    }
	if (input == NULL) {
		printf("FILE_READ_ERROR\n");
		return(FILE_READ_ERROR);
	}
	else {
		first = fgets(first, MAX_LENGTH_OF_STR + 1, input);
		second = fgets(second, MAX_LENGTH_OF_STR + 1, input);
		printf("%s", first);
		printf("%s", second);
	}

	
	int size_in_each_process = MAX_LENGTH_OF_STR / size;
	first_sub = (char*) calloc(size_in_each_process + 1, sizeof(char));
	second_sub = (char*) calloc(MAX_LENGTH_OF_STR + 1, sizeof(char));

	strncpy(first_sub, first + rank * size_in_each_process, size_in_each_process);
	strncpy(second_sub, first + rank * size_in_each_process, size_in_each_process);
	errno = 0;
   
   	int first_num, second_num;
   	into_int(first_sub, &first_num);
   	into_int(second_sub, &second_num);
   	
   	int sum_not_trans, transfer_not_trans, sum_trans, transfer_trans;
   	int max = 10;
   	for (i = 1; i < size_in_each_process; i++)
   		max *= 10;

   	subjective_sum(&first_num, &second_num, size_in_each_process, &transfer_not_trans, &transfer_trans, &sum_not_trans, &sum_trans, max);
  
	int result; 

	if (rank == size - 1) {
		result = sum_not_trans;
		MPI_Send(&transfer_not_trans, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);	
   	}
   	else {
   	int trans; 
   		MPI_Recv(&trans, 1, MPI_INT, rank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
   		if (!rank) {
   			if (trans){
   				if (transfer_trans)
   					result = sum_trans + max;
   				else
   					result = sum_trans;
   			}
   			else {
   				if (transfer_not_trans)
   					result = sum_not_trans + max;
   				else
   					result = sum_not_trans;
   			}
   		}
   		else {
   			if (trans) {
   				result = sum_trans;
   				MPI_Send(&transfer_trans, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);	
   			}
   			else {
   				result = sum_not_trans;
   				MPI_Send(&transfer_not_trans, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);	
			}
		}
	}

	char *string_result = (char*) calloc(size_in_each_process + 1, sizeof(char));
	into_str(string_result, &result, size_in_each_process, rank);


	int nothing;
	if (!rank) {
		fwrite(string_result, sizeof(char) * strlen(string_result), 1, output);
		fclose(output);
		MPI_Send(&nothing, 0, MPI_INT, 1, 0, MPI_COMM_WORLD);	
	}
	else {
		MPI_Recv(&nothing, 0, MPI_INT, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);

		int offset;
		fseek(output, -offset, SEEK_END);

		fwrite(string_result, sizeof(char) * strlen(string_result), 1, output);
		fclose(output);
		if (rank != size - 1)
			MPI_Send(&nothing, 0, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);	
	}

	fclose(input);

	free(string_result);
	free(first);
	free(second);
	free(first_sub);
	free(second_sub);
	MPI_Barrier(MPI_COMM_WORLD); // end of timer
	full_time += MPI_Wtime();

	if (!rank)
		printf("Full time = %.7lf\n", full_time);

	MPI_Finalize();
}