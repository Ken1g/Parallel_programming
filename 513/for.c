#include <omp.h>

int main()
{


#pragma omp parallel for lastprivate(res) shedule(dynamic)
	for (i = 0; i < N; ++i) 
	{
		int k = 1000;
		printf("%d %d\n", res = omp_get_thread_num(), i); 
	}

return 0;
}
