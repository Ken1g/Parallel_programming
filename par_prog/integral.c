#include <pthread.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>

#define DELTA_MIN 0.0001

typedef struct
{
	float start;
	float finish;
	float* result;
} line_seg;

void* integrate(void* seg)
{
	line_seg* l_s = (line_seg*) seg;
	int i;
	float aim = l_s->start;

	while(aim < l_s->finish)
	{
		*(l_s->result) += DELTA_MIN * aim * aim;
		aim += DELTA_MIN;
	}
	return NULL;
}

int main()
{
	int i, n;
	clock_t time;
	line_seg* full_seg;
	float final_result = 0;
	float mark;
	pthread_t* threads;

	scanf("%d", &n);
	time = clock();
	full_seg = (line_seg*) calloc(n, sizeof(line_seg));
	threads = (pthread_t*) calloc(n, sizeof(pthread_t));
	mark = 0;
	for (i = 0; i < n; i++)
	{
		full_seg[i].start = mark;
		mark += (float) 4 / (float) n;
		full_seg[i].finish = mark;
		full_seg[i].result = &final_result;
		pthread_create(&(threads[i]), NULL, integrate, &full_seg[i]);
	}
	for(i = 0; i < n; i++)
		pthread_join(threads[i], NULL);
	free(full_seg);
	free(threads);
	printf("ans = %f", final_result);
	time = clock() - time;
	printf("\ncorrect ans = %f\n", (float) 64 / (float) 3);
	printf("time = %f\n", (double) time/CLOCKS_PER_SEC);

	return 0;
}
	
