/*
 * app6.cxx
 * 
 * Copyright 2014 Neal Friedman <neal@Saturn>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <mpi.h>
#include <iostream>

int main(int argc, char **argv)
{
	MPI_Init(NULL, NULL);
	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int world_rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	std::cout << "rank " << world_rank << " size " << world_size << std::endl;
	
	// code for 03 goes here
	// lets imagine we have a local sum in each node that is 5 and we 
	// want to reduce that to a single value in the root node that is to
	// be displayed to the user
	int our_sum = 5;
	int total_sum = 0;
	MPI_Reduce(&our_sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	std::cout << "rank " << world_rank << " our_sum, total_sum: " << our_sum << ", " << total_sum << std::endl;
	MPI_Finalize();
	return 0;
}

