#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int num;
} str;

unsigned long long rdtscl(void)
{
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));                        
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );  
}

void *doing(void* str) {
}

int main(int argc, char** argv) {
	pthread_t *threads;
	int i, j;
	int N = 50;
	unsigned long long start, end;

	threads = (pthread_t*) calloc(N, sizeof(pthread_t));
	str* strs = (str*) calloc(N, sizeof(str));

	
	start = rdtscl();
	for (j = 0; j < N; j++) {
		pthread_create(&(threads[j]), NULL, doing, &strs[j]);
	}
	end = rdtscl();
	printf("creat %llu\n", (end - start) / N);

	
	start = rdtscl();
	for (j = 0; j < N; j++) {
		pthread_join(threads[j], NULL);
	}
	end = rdtscl();
	printf("join %llu\n", (end - start) / N);

	free(threads);
	free(strs);
}
