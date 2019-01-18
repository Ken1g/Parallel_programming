#define INCORRECT_INPUT		-100
#define NOT_A_NUMBER 		-101
#define OK					 100

#define TEST_PASSED 		 102
#define TEST_FAILED			-102

#define GREEN				 1
#define RED					 0

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <mpi.h>

typedef struct {
	int start;
	int end;
	int* elements;
} sort_path;

int cmp_exc(int* elements, int pos) {
	if (elements[pos] > elements[pos + 1]) {
		int temp = elements[pos];
		elements[pos] = elements[pos + 1];
		elements[pos + 1] = temp; 
	}
	return OK;
}

void* sort(void* path) {
	int i;
	sort_path* my_path = (sort_path*) path;
	int start = my_path->start;
	int end = my_path->end;
	int* elements = my_path->elements;
	
	for (i = start; i < end; i+= 2)
		cmp_exc(elements, i);
}

void set_color(int color) {
	if (!color)
		printf("\033[1;31m");
	else
		printf("\033[1;32m");
} 

int correctness_checker(int* elements, int size) {
	int i;
	int current = elements[0];

	for (i = 1; i < size; i++) {
		if (elements[i] < current) {
			set_color(RED);
			printf("TEST_FAILED\n");
			return TEST_FAILED;
		} 
	}
	set_color(GREEN);
	printf("TEST_PASSED\n");
	return TEST_PASSED;
}

int main(int argc, char** argv) {
	char *endptr;
	int i, d, j;
	long N, n_elements;
	FILE *input, *output;
	pthread_t* threads;
	double MPI_time = 0.0;
	
	MPI_Init(&argc, &argv);

	MPI_time -= MPI_Wtime();

	input = fopen("input_1.txt", "rb");
	output = fopen("output.txt", "wba");

	if (argc != 2) {
		set_color(RED);
		printf("INCORRECT_INPUT\n");
		return INCORRECT_INPUT;
	}	
	errno = 0;
	N = strtol(argv[1], &endptr, 0);
	if ((errno == ERANGE && (N == LONG_MAX || N == LONG_MIN)) || (errno != 0 && N == 0)) {
		set_color(RED);
		printf("INCORRECT_INPUT\n");
		return INCORRECT_INPUT;
	}
	if (*endptr) {
		set_color(RED);
		printf("NOT_A_NUMBER\n");
		return NOT_A_NUMBER;
	}
	
	fscanf(input, "%ld", &n_elements);
	int odd = n_elements % 2;
	int *elements = (int*) calloc(n_elements, sizeof(int));
	for (i = 0; i < n_elements; i++)
		fscanf(input, "%d", elements + i);

	int *num_of_pairs_in_thread = (int*) calloc(N, sizeof(int));
	int num_of_pairs = n_elements / 2;
	if (N > num_of_pairs) {
		N = num_of_pairs; 
		for (i = 0; i < N; i++)
			num_of_pairs_in_thread[i] = 1;
	}
	else {
		int rem = num_of_pairs % N;
		int pairs_in_process = num_of_pairs / N;
		for (i = 0; i < rem; i++)
			num_of_pairs_in_thread[i] = pairs_in_process + 1;
		for (i = rem; i < N; i++)
			num_of_pairs_in_thread[i] = pairs_in_process;
	}

	threads = (pthread_t*) calloc(N, sizeof(pthread_t));
	sort_path* paths = (sort_path*) calloc(N, sizeof(sort_path));

	int pointer;
	for (i = 1; i <= n_elements; i++) {
		if (i % 2) {
			pointer = 0;
			for (j = 0; j < N; j++) {
				paths[j].start = pointer;
				pointer += num_of_pairs_in_thread[j] * 2;
				paths[j].end = pointer;
				paths[j].elements = elements;
				pthread_create(&(threads[j]), NULL, sort, &paths[j]);
			}
		}
		else {
			pointer = 1;
			if (odd) {
				for (j = 0; j < N; j++) {
					paths[j].start = pointer;
					pointer += num_of_pairs_in_thread[j] * 2;
					paths[j].end = pointer;
					paths[j].elements = elements;
					pthread_create(&(threads[j]), NULL, sort, &paths[j]);
				}
			}
			else {
				for (j = 0; j < N; j++) {
					paths[j].start = pointer;
					if (j == N - 1)
						pointer += (num_of_pairs_in_thread[j] - 1) * 2;
					else
						pointer += num_of_pairs_in_thread[j] * 2;
					paths[j].end = pointer;
					paths[j].elements = elements;
					pthread_create(&(threads[j]), NULL, sort, &paths[j]);
				}

			}
		}
		for (j = 0; j < N; j++)
			pthread_join(threads[j], NULL);
	}

	free(threads);
	free(num_of_pairs_in_thread);
	free(paths);

	for (i = 0; i < n_elements; i++)
		fprintf(output, "%d ", elements[i]);

	MPI_time += MPI_Wtime();
	printf("MPI time = %.7lf\n", MPI_time); 
	
	correctness_checker(elements, n_elements);

	MPI_Finalize();
	free(elements);
	fclose(input);
	fclose(output);
}