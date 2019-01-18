#include <stdio.h>
#include <stdlib.h>

unsigned long long rdtscl(void)
{
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));                        
    return ((unsigned long long)lo) | (((unsigned long long)hi)<<32);  
}

int main()
{
	int N, i, j, k, p = 0;
	int rep = 1024 * 1024 * 32; 
	int rep2 = 10;
	int K = 256 * 1024;
	unsigned long long start, end, res;

	for (j = 1; j < 32; ++j) {
		res = 0;
		for (k = 0; k < rep2; k++) {
			N = 256 * 1024 * j;
			int* ints = (int*) calloc(N, sizeof(int)); // 1 MB * j
			start = rdtscl();
			for (i = 0; i < rep; ++i) {
				ints[p]++;
				p += K; // offset = 1 MB
				if (p >= N) {
					p = 0;
				}	

			}
			end = rdtscl();
			res += (end - start);
			free(ints);
		}
		printf("N = %d, time = %llu\n", j, res);
	}

	return 0;
}
