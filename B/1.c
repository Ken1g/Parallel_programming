#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int num;
} str;

void *doing(void* str) {
	printf("wow\n");
}

int main(int argc, char** argv) {
	pthread_t *threads;
	int i, j;
	int N = 5;

	threads = (pthread_t*) calloc(N, sizeof(pthread_t));
	str* strs = (str*) calloc(N, sizeof(str));

	for (j = 0; j < N; j++) {
		pthread_create(&(threads[j]), NULL, doing, &strs[j]);
	}

	for (j = 0; j < N; j++) {
		pthread_join(threads[j], NULL);
	}

	free(threads);
	free(strs);
}
