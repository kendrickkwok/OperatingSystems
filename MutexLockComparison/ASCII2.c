//----------Kendrick Kwok
//----------Coding Assignment #4
//----------912351666
//----------CSC 415

//This program performs no synchronization
//--forming an ASCII Count with a file
//----This is to compare no synchornization with syncrhonization

//include needed to use the write function
#include<unistd.h>
//include needed to use the sprintf function
#include<stdio.h>
//includes thread into the program 
#include <pthread.h>
#include <stdlib.h>
//set number of threads to 8
#define nTHREADS 8
//sets the limit of how many each thread is in charge of 
#define limit 150

//Global variables needed to access in program
int occurenceArray[128];
char buffer[65536];
void printArray();
int i = 0;
int sum = 0;

typedef struct ChildThread{
	//Create a name for the thread		
	int badge;	
	//Create the start of the buffer
	int startOfBuffer;
	//Find the end of the buffer
	int endOfBuffer;	
} define_thread_type;

//method to be read by single threads 
void *count(void *param)
{
	//assign thread_type to pointer
	define_thread_type *thread_type = (define_thread_type*)param;
	
	//assign single thread a name and indexes 
	int countBadge = thread_type->badge;
	int countStartOfBuffer = thread_type->startOfBuffer;
	int countEndOfBuffer = thread_type->endOfBuffer;

	//for everything in the buffer, add one to the occurence in the index 
	//----multidemensional array should all hold values of 0
	for (int i = countStartOfBuffer; i <= countEndOfBuffer; i++) {
		for (int j = 0; j < 128; j++) {
			if (buffer[i] == j){
				occurenceArray[j] = occurenceArray[j]+1;			
			}	
		}
	}
}

int main(int argc, char **argv)
{
	//if arguments are not recognize, throw out error 
	if (argc != 2) {
        printf(" Arguments are wrong\n\n\tUsage: ASCII.c\n");
        return 1;
   	}

	//**********Reading the file 
	int i, size;
	FILE *fp;
	fp = fopen(argv[1], "r");
	fread(buffer,1, (sizeof buffer)-1, fp);
	//********************************************************************
	

	//***********Variable Declarations 
	//initialize an array of 8 rows and 128 columns called countMyArray	
		for (int i = 0; i < 128; i++){
			occurenceArray[i] = 0;
		}
	//passing struct into thread_index for partitioning of the buffer
	define_thread_type thread_index[nTHREADS];
	
	//defining an array of threads for parsing
	pthread_t arrayOfThreads[nTHREADS];
	
	//create the attributes for the thread
	pthread_attr_t attr;
	
	//takes the reference of the attributes, gets default attributes
	pthread_attr_init(&attr);

	//sizes of bytes read
	ssize_t read_bytes = 0;

	//buffer thread
	int singleThread = 0;

	//starting point
	int startingPoint;
	//****************************************************
	
	//Partition the bytes into seperate threads
	//singleThread will hold the bytes 64k/8
	startingPoint = 0;
	singleThread = ((double)sizeof buffer / nTHREADS);
	
	//initialize all the threads 
	for (int tally = 0; tally < nTHREADS; tally++) {
	
	//assign an id for the thread
	thread_index[tally].badge = tally;	

	//assigning the start value for the thread --- starts from 0
	//Keep adding starting point, this way the thread knows where to start in buffer
	//.....ie 8192 bytes, 16384 bytes.......
	thread_index[tally].startOfBuffer = startingPoint;	
	startingPoint = startingPoint + singleThread;
	
	//assigned end of buffer -- starting point of next thread subtracted by one
	thread_index[tally].endOfBuffer = startingPoint-1;

	//create the read with the struct that holds the partitions
	//----struct needed because an thread can only pass one argument
	pthread_create(&arrayOfThreads[tally], &attr, count, &thread_index[tally]);
	}
	
	//Wait before all the threads finish before exiting program 
	for (int tally = 0; tally < nTHREADS; tally++) {
		pthread_join(arrayOfThreads[tally],NULL);
	}
	//sum all of the arrays and print out an array with desired results	
	printArray();

}
 
void printArray()
{

	//print out the occurence in occurenceArray
	//print our hexdecimals for 0-33
	for (int i = 0; i < 33; i++){
	printf("%d occurences of 0*%d\n", occurenceArray[i],i);	
	}
	//print out ASCII Characters for 32-127
	 for (int i = 32; i < 127; i++) {
	printf("%d occurences of %c\n", occurenceArray[i],i);
	}
}

