#define INCORRECT_INPUT         -100
#define NOT_A_NUMBER            -101

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <omp.h>

int main(int argc, char **argv) {
	long i, N;
	int myid;
	char* endptr;
	long double step, result = 0;

	if (argc != 2)
        {
                printf("INCORRECT_INPUT\n");
                return INCORRECT_INPUT;
        }
        errno = 0;
        N = strtol(argv[1], &endptr, 0);
        if ((errno == ERANGE && (N == LONG_MAX || N == LONG_MIN)) || (errno != 0 && N == 0))
        {
                printf("INCORRECT_INPUT\n");
                return INCORRECT_INPUT;
        }
        if (*endptr)
        {
                printf("NOT_A_NUMBER\n");
                return NOT_A_NUMBER;
	}

        N = strtol(argv[1], &endptr, 0);
	omp_set_num_threads(N);

#pragma omp parallel for private(i) shared(result) private(step)
	for (i = 1; i <= 1000000; ++i) {
		step = 1.0 / i;
#pragma omp atomic
		result += step;
		
	}
	
//printf("%.20Lf\n", result);
result = 0;

#pragma omp parallel for reduction(+:result) private(step)
	for (i = 1; i <= 1000000; ++i) {
		step = 1.0 / long double i;
		result += step;
	}



printf("%.20Lf\n", result);

return 0;
}
