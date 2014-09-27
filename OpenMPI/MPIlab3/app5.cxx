// Broadcasting

#include <mpi.h>
#include <iostream>

int main(int argc, char **argv)
{
	// initialise the MPI library
	MPI_Init(NULL, NULL);
	
	// determine the world size
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	
	// determine our rank in the world
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	
	// print out the rank and size
	std::cout << "rank " << world_rank << " size " << world_size << std::endl;
	
	// 03 code goes here
	if(world_rank == 0) 
	{
		// broadcast a message to all the other nodes
		int message = 0xDEADBEEF;
		MPI_Bcast(&message, 1, MPI_INT, 0, MPI_COMM_WORLD);
		std::cout << "rank 0 broadcasting " << message << std::endl;
	} 
	else 
	{
		// recieve a message from the root
		int message = 0;
		MPI_Bcast(&message, 1, MPI_INT, 0, MPI_COMM_WORLD);
		std::cout << "rank " << world_rank << " recieved broadcast of " << message <<
			std::endl;
	}
	// finalize the MPI library
	MPI_Finalize();
	return 0;
}

