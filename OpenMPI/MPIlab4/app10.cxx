/*
 * app10.cxx GATHER
 * 
 * Copyright 2014 Neal Friedman <neal@Saturn>
 * 
 */


#include <iostream>
#include <mpi.h>
#include <cstdlib>

int main(int argc, char **argv)
{
	MPI_Init(NULL, NULL);
	int world_size, world_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	
	// arrays that we need for communicating data
	int *total_array = new int[40];
	int *partition = new int[40 / world_size];
	
	// all processes should generate a partition of data to be 
	// communicated to the node that it is gather and print it out to
	// console
	srand(world_rank);
	std::cout << "rank " << world_rank << " numbers: ";
	for(unsigned int i = 0; i < 40 / world_size; i++)
	{
		partition[i] = rand() % 10;
		std::cout << partition[i] << ", ";
	}
	std::cout << std::endl;
	
	// run the gather operation and print out the total partition that
	// was recieved
	MPI_Gather(partition, 40 / world_size, MPI_INT, total_array, 
		40 / world_size, MPI_INT, 0, MPI_COMM_WORLD);
	if(world_rank == 0)
	{
		std::cout << "total data----------" << std::endl;
		for(unsigned int i = 0; i < 40; i++)
		{
			std::cout << total_array[i] << ", ";
			if(i % 10 == 9)
				std::cout << std::endl;
		}
	}
	MPI_Finalize();
	
	// clear up our memory before we finish
	delete total_array;
	delete partition;
}

