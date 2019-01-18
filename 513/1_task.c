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



#pragma omp parallel for ordered private(i)
	for (i = 0; i < N; ++i) {
		#pragma omp ordered
		myid = omp_get_thread_num();
		#pragma omp ordered
		printf("ID = %d, i = %ld\n", myid, i);
	}


return 0;
}
