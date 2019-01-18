run:
	mpicc 1p.c -lm
	mpirun -np 1 a.out
	mpicc 1.c -lm
	mpirun -np 2 a.out
	diff 1 2
