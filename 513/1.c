#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	int N = 5;
	int res = 0;
	int i;

	omp_set_num_threads(N);
#pragma omp parallel
{
	#pragma omp for 
	for (i = 0; i < N; i++) {
		res += 1;	
	}
}
printf("%d\n", res);


return 0;
}
