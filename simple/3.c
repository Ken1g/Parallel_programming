#define INCORRECT_INPUT         -100
#define NOT_A_NUMBER            -101

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <malloc.h>
#include <math.h>

unsigned long long rdtscl(void)
{
	unsigned int lo, hi;
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));                        
	return ((unsigned long long)lo) | (((unsigned long long)hi)<<32);  
}

int main(int argc, char** argv) {
	long i;
	long *N = (long*) calloc(2, sizeof(long));
	char *endptr;

	if (argc != 3)
	{
		printf("INCORRECT_INPUT\n");
		return INCORRECT_INPUT;
	}
	errno = 0;
	for (i = 0; i < 2; ++i) {
		N[i] = strtol(argv[i + 1], &endptr, 0);
		if ((errno == ERANGE && (N[i] == LONG_MAX || N[i] == LONG_MIN)) || (errno != 0 && N[i] == 0))
		{
			printf("INCORRECT_INPUT\n");
			return INCORRECT_INPUT;
		}
		if (*endptr)
		{
			printf("NOT_A_NUMBER\n");
			return NOT_A_NUMBER;
		}
	}

	long long double results = (long long double*) calloc(N[0], sizeof(long long double));
	long long double last_ = (long long double*) calloc(N[0], sizeof(long long double));
	 
#pragma omp parallel for reduction(+:sum)
	for (i = 0; i < N[1]; ++i) {
 	
	}
	




	free(N);	


	return 0;
}

