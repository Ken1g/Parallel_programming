#define OK					 100
#define FILE_READ_ERROR 	-100
#define INCORRECT_INPUT 	-101
#define NOT_A_NUMBER       	-102

#define MAX_LENGTH_OF_STR 	 1024 * 8 * 8 * 8 * 4
#define MAX_BLOCK			 100

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

int calculate(char* first_numbers, char* second_numbers, char* inprocess_result_trans, char* inprocess_result_not_trans, 
			  int number_of_nums, int* result_carry, int* result_carry_trans) {
	int t;
	int t_trans;
	char** temp = (char**) calloc(number_of_nums, sizeof(char*));
	char** temp_trans = (char**) calloc(number_of_nums, sizeof(char*));
	for (int i = 0; i < number_of_nums; i ++) {
		temp[i] = (char*) calloc(3, sizeof(char));
		temp_trans[i] = (char*) calloc(3, sizeof(char));
	}
	int trans_trans = 1;
	int trans = 0;
	for (int i = number_of_nums - 1; i >= 0; i--) {
		t_trans = first_numbers[i] + second_numbers[i] + trans_trans;
		t = first_numbers[i] + second_numbers[i] + trans;
		sprintf(temp[i], "%d", t);
		sprintf(temp_trans[i], "%d", t_trans);

		trans = 0;
		trans_trans = 0;
		if (t > MAX_BLOCK) {
			memmove(temp[i], temp[i] + 1, strlen(temp[i]));
			trans = 1;
		}
		if (t_trans > MAX_BLOCK) {
			memmove(temp_trans[i], temp_trans[i] + 1, strlen(temp_trans[i]));
			trans_trans = 1;
		}
		if ((i == 0) && (trans == 1))
			*result_carry = 1;
		else
			*result_carry = 0;
		if ((i == 0) && (trans_trans == 1))
			*result_carry_trans = 1;
		else
			*result_carry_trans = 0;
	}
	for (int i = 0; i < number_of_nums; i++) {
		strcat(inprocess_result_trans, temp_trans[i]);
		free(temp_trans[i]);
		strcat(inprocess_result_not_trans, temp[i]);
		free(temp[i]);
	}
	free(temp);
	free(temp_trans);

	return OK;
}

int main(int argc, char** argv) {
	FILE *input, *output;
	int i, rank, size;
	char *first_sub, *second_sub, *first, *second;
	MPI_Status Status;
	double full_time = 0.0;

	first = (char*) calloc(MAX_LENGTH_OF_STR + 2, sizeof(char));
	second = (char*) calloc(MAX_LENGTH_OF_STR + 2, sizeof(char));

/***
		Initilization Section
 ***/

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Barrier(MPI_COMM_WORLD); // start of timer
	full_time -= MPI_Wtime();

/***
		Reading Section
 ***/
	
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
			first = fgets(first, MAX_LENGTH_OF_STR + 2, input);
			second = fgets(second, MAX_LENGTH_OF_STR + 2, input);
		}
	}


/***
		Main Section
 ***/

	int size_in_each_process = MAX_LENGTH_OF_STR / size;
	first_sub = (char*) calloc(size_in_each_process + 1, sizeof(char));
	second_sub = (char*) calloc(size_in_each_process + 1, sizeof(char));

	MPI_Scatter(first, size_in_each_process, MPI_CHAR, first_sub, size_in_each_process, MPI_CHAR, 0, MPI_COMM_WORLD); 
	MPI_Scatter(second, size_in_each_process, MPI_CHAR, second_sub, size_in_each_process, MPI_CHAR, 0, MPI_COMM_WORLD);


	int number_of_nums = size_in_each_process / 2;
	char* first_numbers = (char*) calloc(number_of_nums + 1, sizeof(char));
	char* second_numbers = (char*) calloc(number_of_nums + 1, sizeof(char));
	char* t_string = (char*) calloc(3, sizeof(char));
	char* endptr;
	for (i = 0; i < number_of_nums; i++) {
		first_numbers[i] = strtol(strncpy(t_string, first_sub + 2 * i, 2), &endptr, 0);
		second_numbers[i] = strtol(strncpy(t_string, second_sub + 2 * i, 2), &endptr, 0);
	}

	char* inprocess_result_trans = (char*) calloc(size_in_each_process + 2, sizeof(char));
	char* inprocess_result_not_trans = (char*) calloc(size_in_each_process + 2, sizeof(char));
	int result_carry, result_carry_trans;

	
	calculate(first_numbers, second_numbers, inprocess_result_trans, inprocess_result_not_trans, number_of_nums, &result_carry, &result_carry_trans);
	
	char* result = (char*) calloc(size_in_each_process + 2, sizeof(char));
	char* start = (char*) calloc(MAX_LENGTH_OF_STR + 3, sizeof(char));

	if (rank == size - 1) {
		strcat(result, inprocess_result_not_trans);
		MPI_Send(&result_carry, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
	}	
	else {
		int trans;
		MPI_Recv(&trans, 1, MPI_INT, rank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, &Status);
		if (!rank) {
			if (trans) {
				strcat(result, inprocess_result_trans);
				if (result_carry_trans)
					strcat(start, "1");
			}
			else {
				strcat(result, inprocess_result_not_trans);
				if (result_carry)
					strcat(start, "1");
			}
		}
		else {
			if (trans) {
				strcat(result, inprocess_result_trans);
				MPI_Send(&result_carry_trans, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
			}
			else {
				strcat(result, inprocess_result_not_trans);
				MPI_Send(&result_carry, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD);
			}
		}
	}

	char *final_string = (char*) calloc(MAX_LENGTH_OF_STR + 2, sizeof(char));

	MPI_Gather(result, size_in_each_process, MPI_CHAR, final_string, size_in_each_process, MPI_CHAR, 0, MPI_COMM_WORLD);
	if (!rank)
		strcat(start, final_string);
	free(final_string);

/***
		Output Section 
 ***/

	if (!rank) {
		fwrite(start, sizeof(char) * strlen(start), 1, output);
		fclose(input);
		fclose(output);
	}

/***
		End Section
 ***/

	free(first);
	free(second);
	free(first_sub);
	free(second_sub);
	free(result);
	free(start);
	free(first_numbers);
	free(second_numbers);
	free(t_string);
	MPI_Barrier(MPI_COMM_WORLD); // end of timer
	full_time += MPI_Wtime();

	if (!rank)
		printf("Full time = %.7lf\n", full_time); 

	MPI_Finalize();
}