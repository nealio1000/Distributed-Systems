/*
 * matrixMult.cxx
 * 
 * Copyright 2014 Neal Friedman <neal@Saturn>
 * 
 */


#include <iostream>
#include <mpi.h>
#include <vector>
#include <cstdlib>
#include <fstream>

using namespace std;

void multMatrix(double stripe[], double matrixb[], double matrixc[], 
					int stripeSize, int matrixSize, int rowSize)
{
	
		for(int j = 0, i = 0; i < matrix
		Size; j++)
		{
			
				
				matrixc[j] += stripe[j] * matrixb[i];
				i += stripeSize;
			
			i++;
		} 
		
		for(int i = 0; i < stripeSize; i++)
		{
			cout << matrixc[i] << ", ";
			if(i % rowSize == (rowSize - 1))
				cout << endl;
		}
		cout << endl;
}



// task for the participant
void participant(int world_rank, int world_size)
{
	int errorMessage;
	int rowSize;
	int matrixSize;
	int stripeSize;
	
	MPI_Bcast(&errorMessage, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	if(errorMessage == 1)
	{	
		//cout << "Message received: Arguments successful (" << errorMessage << ")" << endl; 
		
		MPI_Bcast(&rowSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&matrixSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&stripeSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
		
		//cout << "Row size received: " << rowSize << endl;
		//cout << "Full matrix size received: " << matrixSize << endl;
		//cout << "Stripe size received: " << stripeSize << endl;
		
		double matrixb [matrixSize];
		double stripe [stripeSize];
		
		MPI_Scatter(NULL, 0, MPI_DOUBLE, &stripe, stripeSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Bcast(matrixb, matrixSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		
		
				
	}
	else
	{
		cout << "Message received: Program Failure (" << errorMessage << ")" << endl; 
		MPI_Finalize();
		exit(1);
	}
}

// task for the coordinator
void coordinator(int world_rank, int world_size)
{
	string programName;
	string matrixA;
	string matrixB;
	int rowSize;
	int matrixSize;
	fstream programFile;
	fstream matrixFileA;
	fstream matrixFileB;
	int errorMessage;
	int stripeSize;
	
	// User enters program name, then opens file
	cout << "Enter Program Name: ";
	getline( cin, programName );
	programFile.open(programName.c_str());
	
	// User enters first matrix filename, opens file and reads it
	cout << "Enter first matrix filename: ";
	getline(cin, matrixA);
	matrixFileA.open(matrixA.c_str());
	
	// User enters first matrix filename, opens file and reads it
	cout << "Enter second matrix filename: ";
	getline(cin, matrixB);
	matrixFileB.open(matrixB.c_str());	
	
	// User enters matrix size. matrix is square so this number says amount of rows and amount of columns
	cout << "Enter matrix size: ";
	cin >> rowSize;
	
	matrixSize = rowSize * rowSize;
	stripeSize = matrixSize / world_size;
	
	double matrixa [matrixSize];
	double matrixb [matrixSize];
	double stripe [stripeSize];
	double matrixc [stripeSize];
	
	if((programFile.is_open()) && (matrixFileA.is_open()) && (matrixFileB.is_open()) && ((rowSize % 8) == 0))
	{
		for(int i = 0; i < (matrixSize); i++)
			matrixFileA >> matrixa[i];
		
		for(int i = 0; i < (matrixSize); i++)
			matrixFileB >> matrixb[i];	
		
		
		
		errorMessage = 1; // 1 if all arguments are succesfful
        
        MPI_Bcast(&errorMessage, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&rowSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&matrixSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(&stripeSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
		MPI_Scatter(matrixa, stripeSize, MPI_DOUBLE, &stripe, stripeSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		MPI_Bcast(matrixb, matrixSize, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		
		multMatrix(stripe, matrixb, matrixc, stripeSize, matrixSize, rowSize);
        
	}
	else
	{
		errorMessage = 0; // Incorrect argument
        MPI_Bcast(&errorMessage, 1, MPI_INT, 0, MPI_COMM_WORLD);
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

