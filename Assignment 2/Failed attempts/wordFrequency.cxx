/*
 * wordFrequency.cxx
 * 
 * Copyright 2014 Neal Friedman <neal@Saturn>
 * 
 */


#include <iostream>
#include <mpi.h>
#include <cstdlib>
#include <fstream>

using namespace std;

// task for the participant
void participant(int world_rank, int world_size)
{
	
}

// task for the coordinator
void coordinator(int world_rank, int world_size)
{
	int array_size = 2048; // define the size of character array
	char *array = new char[array_size]; // allocating an array of 2kb
	int position = 0; // used for position in array
    ifstream myFile;
    myFile.open("words.txt");
    if (myFile.is_open())   // broadcast a 0 if file exists
    {
        int message = 0;
        MPI_Bcast(&message, 1, MPI_INT, 0, MPI_COMM_WORLD);
        cout << "File Found " << endl;
        
        while(!myFile.eof() && position < array_size)
		{
			myFile.get(array[position]); //reading one character from file to array
			position++;
		}
		array[position-1] = '\0'; //placing character array terminating character
		
		cout << "Displaying Array..." << endl << endl;
		//this loop display all the charaters in array till \0
		int length = 0; 
		for(int i = 0; array[i] != '\0'; i++)
		{
			cout << array[i];
			length++;
		}
        cout << "Character length: " << length << endl;
        
        for(int i = 0;array[i] !='\0' && i < length/world_size; i++)
        {
			cout << array[i];
		}
    }
    else  // broadcast a -1 if file doesn't exist
    {
        
        int message = -1;
        MPI_Bcast(&message, 1, MPI_INT, 0, MPI_COMM_WORLD);
        cout << "File Not Found "<< endl;
    }
    
}

int main(int argc, char **argv)
{
	
	MPI_Init(NULL, NULL);
	int world_size, world_rank;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	
	// rank 0 is coordinator. All others are participant
	if(world_rank == 0) 
		coordinator(world_rank, world_size);
	else
		participant(world_rank, world_size); 
	
	MPI_Finalize();
}
