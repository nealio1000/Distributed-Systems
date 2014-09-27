/*
 *app11.cxx
 * 
 * Copyright 2014 Neal Friedman <neal@Saturn>
 * 
 */


#include <iostream>
#include <mpi.h>

int main(int argc, char **argv)
{
	MPI_Init(NULL, NULL);
	int world_size, world_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	
	// ******code for 03 goes here******
	
	// creating a communicator in MPI requires a bit of work but the 
	// first task is to extract a handle for the world communicator as
	// all user defined communicators are subsets of this overall
	// communicator
	MPI_Group world_group;
	MPI_Comm_group(MPI_COMM_WORLD, &world_group);
	
	// ******code for 04 goes here******
	
	// we need to determine the world rank numbers that will be in the 
	// new communicator. in this example we will divide our group into
	// two dynamically
	int *group_ranks = new int[world_size / 2];
	if(world_rank < world_size / 2)
	{
		for(int i = 0; i < world_size / 2; i++)
			group_ranks[i] = i;
	}
	else
	{
		for(int i = world_size / 2, j = 0; i < world_size; i++, j++)
			group_ranks[j] = i;
	}
	
	// print out the group ranks
	std::cout << "rank " << world_rank << " group ranks: ";
	for(int i = 0; i < world_size / 2; i++)
		std::cout << group_ranks[i] << ", ";
	std::cout << std::endl;
	
	// ******code for 05 goes here******
	
	// create a new group out of the ranks and then create the 
	// communicator with that group
	MPI_Group new_group;
	MPI_Group_incl(world_group, world_size / 2, group_ranks, &new_group);
	MPI_Comm sub_comm;
	MPI_Comm_create(MPI_COMM_WORLD, new_group, &sub_comm);
	
	// ******code for 06 goes here******
	
	// get our rank and size in the new communicator and print that out
	
	int new_rank, new_size;
	MPI_Comm_size(sub_comm, &new_size);
	MPI_Comm_rank(sub_comm, &new_rank);
	std::cout<< "rank " << world_rank << " sub comm size and rank: " <<
		new_size << " " << new_rank << std::endl;
	
	// send a simple broadcast message on the new communicator with the
	// world rank
	int message = world_rank;
	MPI_Bcast (&message, 1 , MPI_INT, 0, sub_comm);
	std::cout << "world rank " << world_rank << " sub comm message is " <<
		message << std::endl;
	
	// delete all user define communicators and groups
	MPI_Comm_free(&sub_comm);
	MPI_Group_free(&new_group);
	
	// any memory we allocated must be deallocated
	delete group_ranks;
	
	// we are done with the MPI librarty so we must finalize it
	MPI_Finalize();
}

