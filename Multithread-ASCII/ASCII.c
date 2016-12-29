//----------Kendrick Kwok
//----------Coding Assignment #3
//----------912351666
//----------CSC 415

//This program performs simple system calls to the program 
//-- on a Linux Terminal

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
int countMyArray[nTHREADS][limit];
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
				countMyArray[countBadge][j] = countMyArray[countBadge][j]+1;			
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
	for (int j = 0; j < nTHREADS; j++){	
		for (int i = 0; i < 128; i++){
			countMyArray[j][i] = 0;
		}
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
	//summing all the arrays into one array
        for (int j = 0; j < 128; j++) {
		for (i = 0; i < 8; i++) {
		//i.e [0][0] + [1][0] + [2][0] + [3][0]....
		//added into
		//occurenceArray[0]
        	sum = sum + countMyArray[i][j]; 
		occurenceArray[j] = sum;
        	}
		sum = 0;
    	}

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




///////********** Sample Code **************/////////
/*
kendrick@kendrick-Latitude-D620:~/Desktop$ gcc -pthread -std=c99 ASCII.c
kendrick@kendrick-Latitude-D620:~/Desktop$ ./a.out pok.txt
45739 occurences of 0*0
0 occurences of 0*1
0 occurences of 0*2
0 occurences of 0*3
0 occurences of 0*4
0 occurences of 0*5
0 occurences of 0*6
0 occurences of 0*7
0 occurences of 0*8
0 occurences of 0*9
360 occurences of 0*10
0 occurences of 0*11
0 occurences of 0*12
0 occurences of 0*13
0 occurences of 0*14
0 occurences of 0*15
0 occurences of 0*16
0 occurences of 0*17
0 occurences of 0*18
0 occurences of 0*19
0 occurences of 0*20
0 occurences of 0*21
0 occurences of 0*22
0 occurences of 0*23
0 occurences of 0*24
0 occurences of 0*25
0 occurences of 0*26
0 occurences of 0*27
0 occurences of 0*28
0 occurences of 0*29
0 occurences of 0*30
0 occurences of 0*31
3450 occurences of 0*32
3450 occurences of  
252 occurences of !
0 occurences of "
0 occurences of #
0 occurences of $
0 occurences of %
0 occurences of &
0 occurences of '
0 occurences of (
0 occurences of )
0 occurences of *
0 occurences of +
0 occurences of ,
0 occurences of -
0 occurences of .
0 occurences of /
0 occurences of 0
0 occurences of 1
0 occurences of 2
0 occurences of 3
0 occurences of 4
0 occurences of 5
0 occurences of 6
0 occurences of 7
0 occurences of 8
0 occurences of 9
0 occurences of :
0 occurences of ;
0 occurences of <
0 occurences of =
0 occurences of >
0 occurences of ?
0 occurences of @
0 occurences of A
0 occurences of B
0 occurences of C
0 occurences of D
0 occurences of E
0 occurences of F
0 occurences of G
0 occurences of H
0 occurences of I
0 occurences of J
0 occurences of K
0 occurences of L
0 occurences of M
0 occurences of N
0 occurences of O
0 occurences of P
0 occurences of Q
0 occurences of R
0 occurences of S
0 occurences of T
0 occurences of U
0 occurences of V
0 occurences of W
0 occurences of X
0 occurences of Y
0 occurences of Z
0 occurences of [
0 occurences of \
0 occurences of ]
0 occurences of ^
0 occurences of _
0 occurences of `
970 occurences of a
400 occurences of b
220 occurences of c
240 occurences of d
1540 occurences of e
140 occurences of f
20 occurences of g
1520 occurences of h
1380 occurences of i
690 occurences of j
0 occurences of k
1380 occurences of l
0 occurences of m
0 occurences of n
690 occurences of o
0 occurences of p
0 occurences of q
0 occurences of r
2760 occurences of s
2780 occurences of t
690 occurences of u
0 occurences of v
0 occurences of w
0 occurences of x
0 occurences of y
0 occurences of z
0 occurences of {
0 occurences of |
0 occurences of }
315 occurences of ~
*//////////////////////////////////////////////////////////
