/*
 * app9.cxx SCATTER
 * 
 * Copyright 2014 Neal Friedman <neal@Saturn>
 * 
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
	
	// if we are process zero then we create an array of 40 random 
	// integers
	if(world_rank == 0)
	{
		std::cout << "total array start-------- " << std::endl;
		for(unsigned int i = 0; i < 40; i++)
		{
			total_array[i] = rand() % 10;
			std::cout << total_array[i] << ", ";
			if(i % 10 == 9) std::cout << std::endl;
		}
		std::cout << "total array end----------- " << std::endl;
	}
	
	// run the scatter operation and then display the contents of all 
	// 4 nodes
	
	MPI_Scatter(total_array, 40 / world_size, MPI_INT, partition, 
		40 / world_size, MPI_INT, 0, MPI_COMM_WORLD);
	std::cout << "rank " << world_rank << " partition: ";
	for(unsigned int i = 0; i < 40 / world_size; i++)
		std::cout << partition[i] << ", ";
	std::cout << std::endl;
	
	// we are done with MPI library so we must finalise it
	MPI_Finalize();
	
	// clear up our memory before we finish
	delete total_array;
	delete partition;
}

