/** simple program to test the MPI stuff to see if it works **/

/** includes **/
#include <iostream>
#include <cstlib>
#include <mpi.h>

/** messages for communicating tasks **/
int COMPUTE_AVERAGE= 1;
int SEND_AVERAGE = 2;
int SHUTDOWN = 3;

/** the world rank and size that will be useful in many functions **/
int world_size;
int world_rank;


void master(void)
{
	// the total average of all the averages from the nodes
	float total_average = 0;
	
	// an average that we recieve from a node
	float average = 0;
	
	// ask all three nodes to compute an average
	for(int i = 1; i < world_size; i++)
		MPI_Send(&COMPUTE_AVERAGE, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	
	std::cout << "Master (0): told all slaves to compute" << std::end1;
	
	// ask all three nodes to send their average to us
	for(int i = 1; i < world_size; i++)
	{
		MPI_Send(&SEND_AVERAGE, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
		MPI_Recv(&average, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		total_average += average;
	}
	// take the average of averages and display the result
	std::cout << "Master (0): average result from all slaves is: " << total_average /
		


int main(int argc, char** argv)
{
	// see if we can initialise the mpi library this is always the first 
	// that we must do at the start of an MPI program
	MPI_Init(NULL, NULL);
	
	// one of the most important taks that we have to establish is how
	// many proccesses are in this MPI instance. this will determine who
	// and what we have to communicate with
	MPI_Comm_size(MPI_COMM_World, &world_size);
	
	// another important job is finding out which rank we are. we can 
	// use this rank number to assign separate jobs to different mpi units
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	
	// depending on which rank we call the appropriate function
	if( world_rank == 0)
		master();
	else
		slave();
	
	// before we can end our application we need to finalise everything 
	// so MPI can shut down properly
	MPI_Finalize();
	
	//stand C/C++ thing to do at the end of main
	return 0;
}
