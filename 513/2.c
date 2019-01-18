#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int i;
	int K = 3;

	omp_set_num_threads(K);
#pragma omp parallel for shared(K) private(i)

	for (i = 0; i < K; i++) {
		printf("%d\n", i);
	}

return 0;
}
