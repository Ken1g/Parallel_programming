#define num_elements 	10
#define num_trials 		1000000

#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

void my_bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator) 
{
	int rank;
	MPI_Comm_rank(communicator, &rank);
	int size;
	MPI_Comm_size(communicator, &size);

	if (rank == root) 
	{
		int i;
		for (i = 0; i < size; i++) 
		{
			if (i != rank) 
				MPI_Send(data, count, datatype, i, 0, communicator);
		}
	} 
	else
	{ 
		MPI_Recv(data, count, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
	}
}

/*
void my_scatter(void* send_data, int send_count, MPI_Datatype send_datatype, 
				void* recv_data, int recv_count, MPI_Datatype recv_datatype, 
				int root, MPI_Comm communicator)
{
	int rank;
	MPI_Comm_rank(communicator, &rank);
	int size;
	MPI_Comm_size(communicator, &size);

}
*/


int main(int argc, char** argv)
{
	int rank = 0;
	int size = 0;
	int i;
	double total_my_bcast_time = 0.0;
	double total_mpi_bcast_time = 0.0;
	double total_mpi_scatter_time = 0.0;
	double total_mpi_gather_time = 0.0;
	double total_mpi_reduce_time = 0.0;
	int* data = (int*) malloc(sizeof(int) * num_elements);
	int* rcvd_data = (int*) malloc(sizeof(int) * num_elements);

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	for (i = 0; i <= num_trials; i++) 
	{
		if ((i % 100000 == 0)  && (rank == 0))
			printf("Iter #%d\n", i);
		if (i > 0) 
		{
    		//MPI_Barrier(MPI_COMM_WORLD);
    		total_my_bcast_time -= MPI_Wtime();
    		my_bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);
    		//MPI_Barrier(MPI_COMM_WORLD);
    		total_my_bcast_time += MPI_Wtime();

    		//MPI_Barrier(MPI_COMM_WORLD);
    		total_mpi_bcast_time -= MPI_Wtime();
    		MPI_Bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);
    		//MPI_Barrier(MPI_COMM_WORLD);
    		total_mpi_bcast_time += MPI_Wtime();

    		//MPI_Barrier(MPI_COMM_WORLD);
    		total_mpi_scatter_time -= MPI_Wtime();
    		MPI_Scatter(data, num_elements / size, MPI_INT, rcvd_data, num_elements / size, MPI_INT, 0, MPI_COMM_WORLD); 
    		//MPI_Barrier(MPI_COMM_WORLD);
    		total_mpi_scatter_time += MPI_Wtime();

    		//MPI_Barrier(MPI_COMM_WORLD);
    		total_mpi_gather_time -= MPI_Wtime();
    		MPI_Gather(data, num_elements / size, MPI_INT, rcvd_data, num_elements / size, MPI_INT, 0, MPI_COMM_WORLD);
    		//MPI_Barrier(MPI_COMM_WORLD);
    		total_mpi_gather_time += MPI_Wtime();

    		//MPI_Barrier(MPI_COMM_WORLD);
    		total_mpi_reduce_time -= MPI_Wtime();
    		MPI_Reduce(data, rcvd_data, num_elements / size, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    		//MPI_Barrier(MPI_COMM_WORLD);
    		total_mpi_reduce_time += MPI_Wtime();
    	}
	}

    if (rank == 0) 
    {
    	printf("Data size = %d, Trials = %d\n", num_elements * (int) sizeof(int), num_trials);
    	printf("Avg my_bcast time = %.7lf\n", total_my_bcast_time / num_trials);
    	printf("Avg MPI_Bcast time = %.7lf\n", total_mpi_bcast_time / num_trials);
    	printf("Avg MPI_Scatter time = %.7lf\n", total_mpi_scatter_time / num_trials);
    	printf("Avg MPI_Gather time = %.7lf\n", total_mpi_gather_time / num_trials);
    	printf("Avg MPI_Reduce time = %.7lf\n", total_mpi_reduce_time / num_trials);
	}
  
  	free(data);
  	free(rcvd_data);
	MPI_Finalize();
	return 0;
}
