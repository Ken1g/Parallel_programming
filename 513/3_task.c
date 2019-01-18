#define INCORRECT_INPUT         -100
#define NOT_A_NUMBER            -101

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <pthread.h>
#include <malloc.h>
#include <omp.h>

unsigned long long rdtscl(void)
{
	unsigned int lo, hi;
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));                        
	return ((unsigned long long)lo) | (((unsigned long long)hi)<<32);  
}

int main(int argc, char** argv) {
	int i;
	int *N = (int*) calloc(2, sizeof(int));
	char *endptr;

	if (argc != 3)
	{
		printf("INCORRECT_INPUT\n");
		return INCORRECT_INPUT;
	}
	errno = 0;
	for (i = 0; i < 2; ++i) {
		N[i] = strtol(argv[i + 1], &endptr, 0);
		if ((errno == ERANGE && (N[i] == INT_MAX || N[i] == INT_MIN)) || (errno != 0 && N[i] == 0))
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

	long double *results = (long double*) calloc(N[0], sizeof(long double));
	long double *last_result = (long double*) calloc(N[0], sizeof(long double));
	omp_set_num_threads(N[0]);

	int num;
#pragma omp parallel for private(i) private(num)
	for (i = 0; i < N[1]; ++i) {
		num = omp_get_thread_num();
		last_result[num] = 1,0; 	
		results[num] = 0.0;
	}	

#pragma omp parallel for private(i) private(num)
	for (i = 1; i <= N[1]; ++i) {
		num = omp_get_thread_num();
		last_result[num] *= 1.0 / i;
		results[num] += last_result[num]; 	
	}

	long double ans = results[0];

	for (i = 1; i < N[0]; ++i) {
		ans += results[i] * last_result[i - 1]; 
		last_result[i] *= last_result[i - 1];
	}
	printf("%.20Lf\n", ans);

	free(last_result);
	free(results);
	free(N);	

	return 0;
}

