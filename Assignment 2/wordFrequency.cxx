/*
 * wordFrequencyVector.cxx
 * 
 * Copyright 2014 Neal Friedman <neal@Saturn>
 * 
 */

#include <mpi.h>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

void incrementWord(vector<string>& wordvector, map <string, int> wordmap, 
					string word, string line, int world_rank)
{
	// Extract words from partition and place them in a map where the keys
	// are the words and the values are the counts corresponding to the 
	// frequency of the words in each partition. Then display the words 
	// and their counts per partition
	
	// extract words and put them into a vector
	for(unsigned int i = 0; i < line.length(); i++)
	{
		if(isalpha(line.at(i)))
			word+=line.at(i);
		else
		{
			wordvector.push_back(word);
			word = "";
		}
	}
	
	// take the words from the vector and put them in a map	
	for(unsigned int i = 0; i < wordvector.size(); i++)
	{
		if(wordvector[i] == "")
			wordvector.erase(wordvector.begin() + i);
		
		if(wordmap.count(wordvector[i]) == 1) // word found, increment count
			wordmap[wordvector[i]]++;
			
		else // first instance of word
			wordmap[wordvector[i]] = 1;
	}	
	
	// Display words and their counts	
	map<string,int>::iterator it;
	for (it = wordmap.begin(); it!=wordmap.end(); ++it)
	{
		if(it->first != "")
			cout << "Counts from rank " << world_rank  << " for word: " << it->first << ", count: " << it->second << endl;
	}	
}


// task for the participant
void participant(int world_rank, int world_size)
{
	int fileFoundMessage = 0;	
	
	// Receive message if file has been found or not
	MPI_Bcast(&fileFoundMessage, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if(fileFoundMessage == 0)
	{
		// Initialize variables
		string line;
		vector <string> wordvector;
		string word;
		map <string, int> wordmap;
		int partition_size;
		cout << "Message received by participant: File Found (" << fileFoundMessage << ")" << endl;
	
		// take part in the scatter to get our partition size
		MPI_Scatter(NULL, 0, MPI_INT, &partition_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
		
		// Display the size of the partition received
		cout << "Psize received on rank " << world_rank << " partition size: " << partition_size << endl;
		
		// allocate memory for our partition
		char *partition = new char[partition_size];
		
		// take part in the scatterv to get our data
		MPI_Scatterv(NULL, NULL, NULL, MPI_CHAR, partition, partition_size, MPI_CHAR, 0, MPI_COMM_WORLD);
		
		// Take partition data and place it into a string
		for(int i = 0; i <= partition_size; i++)
			line += partition[i];
		
		// Display the partition data
		cout << "Data received on rank " << world_rank << " data: " << line << endl;
		
		// Create map out of data with values corresponding to counts per word
		incrementWord(wordvector, wordmap, word, line, world_rank);

//*************** EXPERIMENTAL AREA: DANGER! ***************************
				
		// take part in the gather to let the root know how much data to 
		// expect. as a participant we dont need to send any data hence 
		// arguments 4 and 5 are null and zero
		
		//MPI_Gather(&wordcounts.length(), 1, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD);
		
		// take part in the gatherv to send all of our data to the root
		//MPI_Gatherv(wordcountarray, wordcounts.length(), MPI_CHAR, NULL, NULL, NULL, MPI_CHAR, 0, MPI_COMM_WORLD);
//************** END OF DANGER ZONE ************************************
				
		// clean up any allocated data
		delete partition;
	}
	else
	{
		cout << "Message received by participant: File Not Found (" 
				<< fileFoundMessage << ")" << endl << "Aborting Program" 
				<< endl;			
		MPI_Finalize();
		exit(1);
	}
}

void adjustStarts(vector <char> words, int *offsets, int world_size)
{
	// adjust the offsets such that each offset falls on a character
	// that is outside of the ranges [a,z] and [A,Z]
	
	int *length_array = new int[world_size];
	
	for(int i = 0; i < world_size; i++)
	{
		length_array[i]=((words.size()-1) / world_size);
		offsets[i] = offsets[i-1] + length_array[i-1];
	}
	
	for(int i = 1; i < world_size; i++)
	{
		for(unsigned int j = offsets[i]; (words[j] >= 'a' && words[j] <= 
				'z') || (words[j] >= 'A' && words[j] <= 'Z'); j++)
		{
			offsets[i]++;
		}
	}
}

// task for the coordinator
void coordinator(int world_rank, int world_size)
{
	// Initialize variables
	vector<char> words; 
    ifstream myFile;
    int *offsets = new int[world_size];
    int *partition_length = new int[world_size];
    vector <string> wordvector;
	string word;
	map <string,int> wordmap;    
    int partition_size;
    
    // Open File
    myFile.open("words.txt");
    if (myFile.is_open())   
    {
        int fileFoundMessage = 0; // 0 if the file exists
        MPI_Bcast(&fileFoundMessage, 1, MPI_INT, 0, MPI_COMM_WORLD); // broadcast a 0 if file exists
        cout << "Coordinator says: File Found (" << fileFoundMessage << ")" << endl;
        
        // Read text file into Array
        while(!myFile.eof())
			words.push_back(myFile.get());
		
		// Display text from file
		cout << "Displaying Array..." << endl << endl;		
		for(unsigned int i = 0; i < words.size()-1; i++) 
			cout << words[i];
		 
		// Display length of text from file
        cout << "\nCharacter length: " << words.size()-1 << endl; // Print amount of characters to the screen
             
        // Set offsets
        adjustStarts(words, offsets, world_size); // adjust offsets for character length and world size	
		
		// Adjust partition lengths based on offsets
		for(int i = 0; i < world_size-1; i++)
			partition_length[i] = offsets[i+1] - offsets[i];
		partition_length[world_size-1] = (words.size()-1) - offsets[world_size-1];
		
		// send each node their individual partition size and allocate
		// memory for our own partition
		MPI_Scatter(partition_length, 1, MPI_INT, &partition_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
		
		// Display partition size
		cout << "Psize received on rank 0 partition size: " << partition_size << endl;
		
		char *partition = new char[partition_size];
		char *words_array = new char[words.size()-1];
		
		// Convert words vector to words array to make scatter happy
		for(unsigned int i = 0; i < (words.size() - 1); i++)
			words_array[i] = words[i];
		
		// use the scatterv to send the data to each node
		MPI_Scatterv(words_array, partition_length, offsets, MPI_CHAR, 
						partition, partition_size, MPI_CHAR, 0, 
						MPI_COMM_WORLD);
		
		// take partition and place it in a string
		string line;
		for(int i = 0; i <= partition_size; i++)
		{
			line += partition[i];
		}
		
		// display the data in the partition
		cout << "Data received on rank " << world_rank << " data: " << line << endl;
		
		// map words and their corresponding counts from the partition
		incrementWord(wordvector, wordmap, word, line, world_rank);		
		
	//*********** EXPERIMENTAL AREA: DANGER! **********************************
		//string wordcounts;
		
		// we need to generate the offsets for each partition before we take part in the gather
		//MPI_Gather(&wordcounts.length(), 1, MPI_INT, lengths_array, 1, MPI_INT, 0, MPI_COMM_WORLD);
		
		// gather the data from the nodes
		//MPI_Gatherv(partition, wordcounts.length(), MPI_CHAR, counts_array, lengths_array2, offsets_array2, MPI_INT, 0, MPI_COMM_WORLD);
	//********** YOU ARE NOW LEAVING THE DANGER ZONE *****************************
		
		// Clean up any allocated data
		delete partition;
		delete words_array;
		delete partition_length;
		delete offsets;
		
		// Close file we are done with it
		myFile.close();
    }
    else  // broadcast a -1 if file doesn't exist
    {
        int fileFoundMessage = -1;
        MPI_Bcast(&fileFoundMessage, 1, MPI_INT, 0, MPI_COMM_WORLD);
        cout << "Coordinator says: File Not Found ("<< fileFoundMessage << ")" << endl;
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



