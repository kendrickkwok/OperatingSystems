//**Kendrick Kwok**//
//----912351666----//
//-----CSC 415-----//

//Copy the program using cp in BASH// 
//Take two command lind arguments //
//Open the first file and read the contents //
//Check for errors when reading and writing to the files//

/*SAMPLE OUTPUT SHOULD BE
	./mycp file1 fil2
	copied 512 bytes
*////////////////////////

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

//Buffer size is 1024
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]){
	
	//create a file descriptor to read and write files//
	int inputFile, outputFile;
	//read in the number of bytes returned by reading and writing of bytes
	ssize_t bytesRead, bytesOutput;
	//initialize the character buffer for parsing 
	char bufferArray[BUFFER_SIZE];

	//If there are more arguments then needed, output error message 
	if (argc != 3){
		printf ("Error: mycp readFile copiedFile");
		return 1;
	}
	
	//Open a file to be read//
	//Creates a pointer to the files to be read//
	inputFile = open(argv[1], O_RDONLY);
	//if file cannot be opened
	if (inputFile == -1) {
		printf ("Read file cannot be opened. Try again.");
		return 1;
	}
	
	//Create an output file to store the descriptions
	//0644 describes read and write functionality
	//Creates a pointer for the files to be read and written to 
	outputFile = open(argv[2], O_WRONLY | O_CREAT, 0644);
	if (outputFile == -1) {
		printf("Write file cannot be created");
		return 1;
	}

	//copy the proccess into the file
	while ((bytesRead = read(inputFile, &bufferArray, BUFFER_SIZE)) > 0){
		bytesOutput = write(outputFile, &bufferArray, (ssize_t) bytesRead);
		if (bytesRead != bytesOutput){
		printf ("Copying files error. Writing Error!");
		return 1;
		} 
			else{
			printf("Copied %zu bytes.\n", bytesOutput);
			}
	}
	close (inputFile);
	close (outputFile);
	}

//*********************OUTPUT*************************************//
/*kendrick@kendrick-Latitude-D620:~/Desktop$ ./cp file1 file100 
					  Copied 227 bytes.

file1: Hello this is CSC 415 Class! It is almost finals and I am doing this homework. This is just a test output to see 
if this works. If there are more than 1024 bytes, there will still be enough room for me to write more. Goodbye.

file100: Hello this is CSC 415 Class! It is almost finals and I am doing this homework. This is just a test output to see 
if this works. If there are more than 1024 bytes, there will still be enough room for me to write more. Goodbye.
*//////////////////////////////////////////////////////////////////
