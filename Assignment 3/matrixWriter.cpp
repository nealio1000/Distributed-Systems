// simple program to take a matrix and write it to disk in the form of a binary file

// includes that are necessary to make the program work
#include <iostream>
#include <cstdio>
#include <fstream>

// function that generates a matrix in binary form on disk
void generateMatrixFile() 
{
	// generate a matrix of values that need to be written to disk in the form of a one dimensional array this will write out an 8x8 matrix
	double matrix[64] = {
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1
	};

	// open up a file for writing
	FILE *output = NULL;
	output = fopen("matrix.txt", "wb");

	// do a simple fwrite to write the matrix to file
	fwrite(matrix, sizeof(double), 64, output);

	// close the file when we are finished writing
	fclose(output);
}

// the main function of the program
int main(int argc, char** argv) 
{
	generateMatrixFile();
}
