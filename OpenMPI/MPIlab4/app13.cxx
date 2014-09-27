/*
 * app13.cxx GATHERV
 * 
 * Copyright 2014 Neal Friedman <neal@Saturn>
 * 
 * 
 */


#include <iostream>
#include <mpi.h>
#include <cstdlib>

// function that takes in an integer array and prints it out to console
void printArrayToConsole(int *to_print, int print_size)
{
	for(int i = 0; i < print_size; i++)
	{
		std::cout << to_print[i] << ", ";
		//if(i % 10 == 9)
			//std::cout << std::endl;
	}
	std::cout << std::endl;
}

// task for the participant
void participant(int world_rank, int world_size)
{
	// we need to generate some random data		
	int partition_size = (40 / world_size);
	if(world_rank % 2 == 0)
		partition_size++;
	else
		partition_size--;
	int *partition = new int[partition_size];
	srand(world_rank);
	for(unsigned int i = 0; i < partition_size; i++)
		partition[i] = rand() % 10;
	std::cout << "rank " << world_rank << " data: ";
	printArrayToConsole(partition, partition_size);
	
	// take part in the gather to let the root now how much data to 
	// expect. as a participant we dont need to send any data hence
	// arguments 4 and 5 are null and zero
	MPI_Gather(&partition_size, 1, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD);
	
	// take part in the gather to send all of our data to the root
	MPI_Gatherv(partition, partition_size, MPI_INT, NULL, NULL, NULL, 
		MPI_INT, 0, MPI_COMM_WORLD);
	
	// clean up any data we have allocated
	delete partition;
}

// task for the cordinator
void coordinator(int world_rank, int world_size)
{
	// we need to generate some random data as we are node zero
	int partition_size = (40 / world_size) + 1;
	int *partition = new int[partition_size];
	int *num_array = new int[40];
	for(unsigned int i = 0; i < partition_size; i++)
		partition[i] = rand() % 10;
	std::cout << "rank 0 data: ";
	printArrayToConsole(partition, partition_size);
	
	// we need to gather the length of each partition that each node has
	int *lengths_array = new int[world_size];
	
	// we need to generate the offsets for each partition before we
	// take part in the gather
	MPI_Gather(&partition_size, 1, MPI_INT, lengths_array, 1, MPI_INT, 0, MPI_COMM_WORLD);
	std::cout << "rank 0 sizes recieved: ";
	printArrayToConsole(lengths_array, world_size);
	int *offsets_array = new int[world_size];
	offsets_array[0] = 0;
	for(unsigned int i = 1; i < 4; i++)
		offsets_array[i] = offsets_array[i-1] + lengths_array[i-1];
	
	// gather the data from the nodes
	MPI_Gatherv(partition, partition_size, MPI_INT, num_array, 
		lengths_array, offsets_array, MPI_INT, 0, MPI_COMM_WORLD);
	
	// display the data
	std::cout << "rank 0 recieved data: " << std::endl;
	printArrayToConsole(num_array, 40);
	
	// clean up any memory we have allocated
	delete lengths_array;
	delete partition;
	delete num_array;
}

int main(int argc, char **argv)
{
	MPI_Init(NULL, NULL);
	int world_size, world_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	
	// decide which task this node is taking on depending on the rank
	if(world_rank == 0)
		coordinator(world_rank, world_size);
	else
		participant(world_rank, world_size);
	MPI_Finalize();
}

