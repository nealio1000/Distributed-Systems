/*
 * app7.cxx
 * 
 * Copyright 2014 Neal Friedman <neal@Saturn>

 */

#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
int main(int argc, char **argv)
{
	MPI_Init(NULL, NULL);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	std::cout << "rank " << world_rank << " size " << world_size << std::endl;
	
	//**** code goes here ****
	
	// to show off the mpi barrier we will simulate an operation across 
	// many nodes by implementing a random sleep time we will get all 
	// nodes to synchornize before printing out a final message those of
	// you using *nix systems should remove the leading underscore from
	// the call to the sleep command
	srand(world_rank);
	int sleep_time = rand() % 5;
	std::cout << "node " << world_rank << " sleeping for " << sleep_time << " seconds." <<
		std::endl;
	usleep(sleep_time * 100000);
	std::cout << "node " << world_rank << " exiting sleep and synchronising" << std::endl;
	MPI_Barrier(MPI_COMM_WORLD);
	std::cout << "node " << world_rank << " synchronised" << std::endl;
	//**** code ends here ****
	MPI_Finalize();
	return 0;	
}


