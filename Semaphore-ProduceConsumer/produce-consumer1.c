//----------Kendrick Kwok
//----------Coding Assignment #5
//----------912351666
//----------CSC 415

//producer produces goods, while consumer consumes the goods and does something with them
//producer will produce an item, place it into a bounded buffer and print to a screen
//bounded buffer is used to store a temporary hold for the consumer to consume
//use of semaphore is used to prevent synchronization issues 
	//--wait tells the system that system is about to enter a crtical section
	//----signal says that we left a critical section
//use of mutex locks to further synchronization and make sure produce and consume is correct

//defining BUFFER_SIZE to be 16
#define BUFFER_SIZE 16
//defining TRUE to be the value of 1 for semaphore
#define TRUE 1 
//include needed to use the write function
#include<unistd.h>
//include needed to use the sprintf function
#include<stdio.h>
//includes thread into the program 
#include <pthread.h>
//includes semaphores in program
#include <semaphore.h>
//includes standard library for C
#include <stdlib.h>

//create thread IDs and thread attributes
pthread_t tid;
pthread_attr_t attr;
//create pthread_mutex_t called lock 
pthread_mutex_t lock;
pthread_t pthread_self();
//declare semaphores
sem_t occupied, vacant;
//declare the buffer of 16 slots
int buffer[BUFFER_SIZE];
//keep track of the number of items to be produced and printed
int item = 0;
//keep track of the buffer index
int buffer_index = 0;
//keep track of the buffer outdex
int buffer_outdex = 0;
//keep track of number of threads
int nThreads = 0;

//declare the producer and consumer method
void *producer(void *myParameter);
void *consumer(void *myParameter);
int insert_item(int item);
int remove_item();
void printbuffer();
int convertBinary(int number);

//pass in thread number to program
typedef struct ChildThread{
	//Create a name for the thread	
	int itemsProducing;	
	int itemsConsuming;
	int badge;	
	int counters;
} define_thread_type;

void *producer(void *param) {

	int counter = 0;
	//define thread type 
	define_thread_type *thread_type = (define_thread_type*)param;

	int val; 
		
	for(int i = 0; i < thread_type->itemsProducing; i++){		
		//acquire an empty lock		
		sem_wait(&vacant);	
		//create a mutex lock 	
		pthread_mutex_lock(&lock);
		counter = counter+1;
		//Produce the item, (ID * #PRODUCED) + COUNTER		
		item = (thread_type->badge * thread_type->itemsProducing) + counter;  
		printf("%i is Produced\n", item);
		insert_item(item);
		//unlock mutex lock
		pthread_mutex_unlock(&lock);
		//semaphore is full
		sem_post(&occupied);
	}
}

//insert the item into the buffer
int insert_item(int item) {

	//for each element, insert the item 
	if (buffer_index < BUFFER_SIZE) 
	{
		buffer[buffer_index] = item;
		printf("%i - inserted in buffer\n", item);
		printbuffer();
		buffer_index = buffer_index + 1;
		return 0;
	} 
	
	//when the buffer reaches the max size, loop back to the beginning
	//begin to iterate again from 0 
	if (buffer_index == BUFFER_SIZE)
	{ 
		buffer_index = 0;
		buffer[buffer_index] = item;
		printf("%i - inserted in buffer\n\n", item);
		printbuffer();
		buffer_index = buffer_index + 1;
		return 0;		
	}
}

int remove_item() {
	//for each element, remove the item and turn it back to a zero value 
	if (buffer_outdex < BUFFER_SIZE) 
	{
		//buffer[buffer_outdex]
		printf("%i - removed from buffer\n", buffer[buffer_outdex]);
		buffer[buffer_outdex] = 0;
		printbuffer();
		buffer_outdex++;
		return 0;
	} 
	
	//when the item has reached max, loop back to zero
	if (buffer_outdex == BUFFER_SIZE)
	{
		buffer_outdex = 0;
		printf("%i - removed from buffer\n", buffer[buffer_outdex]);
		buffer[buffer_outdex] = 0;
		printbuffer();
		buffer_outdex++;
		return 0;
	}
}

void *consumer(void *param){	
	//define thread type 
	define_thread_type *thread_type = (define_thread_type*)param;
	//calculate how many times consumers need to consume, has to be exact number
	//--nThreads = PRODUCERS * ITEMS PRODUCED
	//----calculateConsumers = nThreads / CONSUMERS	
	int calculateConsumers = nThreads / thread_type->itemsConsuming;
	for(int i = 0; i < calculateConsumers; i++)
	{		
		//acquire an empty lock
		sem_wait(&occupied);
		//create a mutex lock	
		pthread_mutex_lock(&lock);
		//remove the items in the buffer		
		remove_item();
		//unlock mutex lock
		pthread_mutex_unlock(&lock);
		//semaphore is full
		sem_post(&vacant);
	}
}


void setUpApp() {
	//initialize mutex lock
	pthread_mutex_init(&lock, NULL);
	//get the attributes for pthreads	
	pthread_attr_init(&attr);
	//semaphore initialized to vacant, pshared, value
	//--pshared definition : 0 is shared between pthread,
	//----anything else that is non-zero is between processes
	//------Third parameter is the value you want to put in sempahore
	
	//Take the vacant sempahore and initialize it to how many numbers are in BUFFER_SIZE	
	sem_init(&vacant, 0, BUFFER_SIZE);
	//Take the occupied semaphore and intilize it to 0
	sem_init(&occupied, 0 , 0);
}

//print the buffer for parsing
void printbuffer(){
	for (int i = 0; i < BUFFER_SIZE; i++){
		printf("%i ", buffer[i]);	
	}
	printf("\n\n");
}

//convert the numbers to binary number so errors will be avoided
int convertBinary(int number){
	int sum = 1; 
		
	for ( int i = 0; i < number; i++)
		sum = sum * 2;

	return sum;		
}

int main(int argc, char **argv)
{
	//if arguments are not recognize, throw out error 
	if (argc != 4) {
        printf(" Arguments are wrong\n\n\tUsage: producer-consumer.c\n");
        return 1;
   	}

	//take the arguments, and distribute the numbers for each one 
	int nProducers = atoi(argv[1]);	
	int nConsumers = atoi(argv[2]);	
	int nItemsProduced = atoi(argv[3]);

	//convert the numbers to binary numbers 
	nProducers = convertBinary(nProducers);
	nConsumers = convertBinary(nConsumers);
	nItemsProduced = convertBinary(nItemsProduced);

	printf("Number of producers is %i\n", nProducers); 
	printf("Number of consumers is %i\n", nConsumers); 
	printf("Number of items produced is %i\n", nItemsProduced); 

	//How many threads are needed to launch 
	nThreads = nItemsProduced * nProducers;
	printf("Number of items to be produced and consumed is %i\n\n", nThreads); 

	//initialize everything in the produce consume program
	setUpApp();

	//defining an array of threads for parsing
	pthread_t arrayOfThreads[nProducers];
	pthread_t arrayOfThreads1[nConsumers];
	//passing struct into thread_index for partitioning of the buffer
	define_thread_type thread_index[nProducers];
	define_thread_type thread_index1[nConsumers];
	
	//create threads per producer
	for(int NOThreads = 0; NOThreads < nProducers; NOThreads++)
	{
		//create the thread
			//param1 = pointer to pthread structure
			//param2 = pointer to attributes with parameters
			//param3 = function to run in the thread
			//param4 = void * start the thread, pass 0 
			//pthread_create(&tid, &attr, producer, 0);
		thread_index[NOThreads].badge = NOThreads;
		//assign the producers to the thread
		thread_index[NOThreads].itemsProducing = nItemsProduced; 
		//launch the threads for producers
		pthread_create(&arrayOfThreads[NOThreads], &attr, producer, &thread_index[NOThreads]);	
	}


	for(int NOThreads1 = 0; NOThreads1 < nConsumers; NOThreads1++)
	{
		//printf("Ececuted\n");		
		thread_index1[NOThreads1].badge = NOThreads1;
		//assign the consumers and items produced to be passed into the struct 
		thread_index1[NOThreads1].itemsConsuming = nConsumers;
		thread_index1[NOThreads1].itemsProducing = nItemsProduced;
		//launch the threads for consumers  
		pthread_create(&arrayOfThreads1[NOThreads1], &attr, consumer, &thread_index1[NOThreads1]);	
	}

	//Wait before all the threads finish before exiting program 
	//--Wait for all producer threads to complete 
	for (int tally = 0; tally < nProducers; tally++) {
		pthread_join(arrayOfThreads[tally],NULL);
	}
	
	//--Wait for all consumer threads to complete 
	for (int tally = 0; tally < nConsumers; tally++) {
		pthread_join(arrayOfThreads1[tally],NULL);
	}
}

/* SAMPLE OUTPUT ***************************
********************************************
kendrick@kendrick-Latitude-D620:~/Desktop$ gcc -pthread -std=c99 produce-consumer1.c
kendrick@kendrick-Latitude-D620:~/Desktop$ ./a.out 1 2 5 
Number of producers is 2
Number of consumers is 4
Number of items produced is 32
Number of items to be produced and consumed is 64

1 is Produced
1 - inserted in buffer
1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

2 is Produced
2 - inserted in buffer
1 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

3 is Produced
3 - inserted in buffer
1 2 3 0 0 0 0 0 0 0 0 0 0 0 0 0 

4 is Produced
4 - inserted in buffer
1 2 3 4 0 0 0 0 0 0 0 0 0 0 0 0 

5 is Produced
5 - inserted in buffer
1 2 3 4 5 0 0 0 0 0 0 0 0 0 0 0 

6 is Produced
6 - inserted in buffer
1 2 3 4 5 6 0 0 0 0 0 0 0 0 0 0 

7 is Produced
7 - inserted in buffer
1 2 3 4 5 6 7 0 0 0 0 0 0 0 0 0 

8 is Produced
8 - inserted in buffer
1 2 3 4 5 6 7 8 0 0 0 0 0 0 0 0 

9 is Produced
9 - inserted in buffer
1 2 3 4 5 6 7 8 9 0 0 0 0 0 0 0 

10 is Produced
10 - inserted in buffer
1 2 3 4 5 6 7 8 9 10 0 0 0 0 0 0 

1 - removed from buffer
0 2 3 4 5 6 7 8 9 10 0 0 0 0 0 0 

2 - removed from buffer
0 0 3 4 5 6 7 8 9 10 0 0 0 0 0 0 

3 - removed from buffer
0 0 0 4 5 6 7 8 9 10 0 0 0 0 0 0 

4 - removed from buffer
0 0 0 0 5 6 7 8 9 10 0 0 0 0 0 0 

5 - removed from buffer
0 0 0 0 0 6 7 8 9 10 0 0 0 0 0 0 

6 - removed from buffer
0 0 0 0 0 0 7 8 9 10 0 0 0 0 0 0 

7 - removed from buffer
0 0 0 0 0 0 0 8 9 10 0 0 0 0 0 0 

8 - removed from buffer
0 0 0 0 0 0 0 0 9 10 0 0 0 0 0 0 

9 - removed from buffer
0 0 0 0 0 0 0 0 0 10 0 0 0 0 0 0 

10 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

33 is Produced
33 - inserted in buffer
0 0 0 0 0 0 0 0 0 0 33 0 0 0 0 0 

33 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

34 is Produced
34 - inserted in buffer
0 0 0 0 0 0 0 0 0 0 0 34 0 0 0 0 

35 is Produced
35 - inserted in buffer
0 0 0 0 0 0 0 0 0 0 0 34 35 0 0 0 

36 is Produced
36 - inserted in buffer
0 0 0 0 0 0 0 0 0 0 0 34 35 36 0 0 

34 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 35 36 0 0 

35 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 36 0 0 

36 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

37 is Produced
37 - inserted in buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 37 0 

37 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

38 is Produced
38 - inserted in buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 38 

39 is Produced
39 - inserted in buffer

39 0 0 0 0 0 0 0 0 0 0 0 0 0 0 38 

38 - removed from buffer
39 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

39 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

11 is Produced
11 - inserted in buffer
0 11 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

12 is Produced
12 - inserted in buffer
0 11 12 0 0 0 0 0 0 0 0 0 0 0 0 0 

11 - removed from buffer
0 0 12 0 0 0 0 0 0 0 0 0 0 0 0 0 

12 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

40 is Produced
40 - inserted in buffer
0 0 0 40 0 0 0 0 0 0 0 0 0 0 0 0 

41 is Produced
41 - inserted in buffer
0 0 0 40 41 0 0 0 0 0 0 0 0 0 0 0 

40 - removed from buffer
0 0 0 0 41 0 0 0 0 0 0 0 0 0 0 0 

41 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

13 is Produced
13 - inserted in buffer
0 0 0 0 0 13 0 0 0 0 0 0 0 0 0 0 

14 is Produced
14 - inserted in buffer
0 0 0 0 0 13 14 0 0 0 0 0 0 0 0 0 

13 - removed from buffer
0 0 0 0 0 0 14 0 0 0 0 0 0 0 0 0 

14 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

42 is Produced
42 - inserted in buffer
0 0 0 0 0 0 0 42 0 0 0 0 0 0 0 0 

42 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

43 is Produced
43 - inserted in buffer
0 0 0 0 0 0 0 0 43 0 0 0 0 0 0 0 

44 is Produced
44 - inserted in buffer
0 0 0 0 0 0 0 0 43 44 0 0 0 0 0 0 

43 - removed from buffer
0 0 0 0 0 0 0 0 0 44 0 0 0 0 0 0 

44 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

15 is Produced
15 - inserted in buffer
0 0 0 0 0 0 0 0 0 0 15 0 0 0 0 0 

15 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

16 is Produced
16 - inserted in buffer
0 0 0 0 0 0 0 0 0 0 0 16 0 0 0 0 

17 is Produced
17 - inserted in buffer
0 0 0 0 0 0 0 0 0 0 0 16 17 0 0 0 

16 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 17 0 0 0 

17 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

18 is Produced
18 - inserted in buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 18 0 0 

18 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

19 is Produced
19 - inserted in buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 19 0 

20 is Produced
20 - inserted in buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 19 20 

19 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 20 

21 is Produced
21 - inserted in buffer

21 0 0 0 0 0 0 0 0 0 0 0 0 0 0 20 

20 - removed from buffer
21 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

22 is Produced
22 - inserted in buffer
21 22 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

21 - removed from buffer
0 22 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

23 is Produced
23 - inserted in buffer
0 22 23 0 0 0 0 0 0 0 0 0 0 0 0 0 

22 - removed from buffer
0 0 23 0 0 0 0 0 0 0 0 0 0 0 0 0 

24 is Produced
24 - inserted in buffer
0 0 23 24 0 0 0 0 0 0 0 0 0 0 0 0 

23 - removed from buffer
0 0 0 24 0 0 0 0 0 0 0 0 0 0 0 0 

25 is Produced
25 - inserted in buffer
0 0 0 24 25 0 0 0 0 0 0 0 0 0 0 0 

24 - removed from buffer
0 0 0 0 25 0 0 0 0 0 0 0 0 0 0 0 

26 is Produced
26 - inserted in buffer
0 0 0 0 25 26 0 0 0 0 0 0 0 0 0 0 

25 - removed from buffer
0 0 0 0 0 26 0 0 0 0 0 0 0 0 0 0 

45 is Produced
45 - inserted in buffer
0 0 0 0 0 26 45 0 0 0 0 0 0 0 0 0 

46 is Produced
46 - inserted in buffer
0 0 0 0 0 26 45 46 0 0 0 0 0 0 0 0 

47 is Produced
47 - inserted in buffer
0 0 0 0 0 26 45 46 47 0 0 0 0 0 0 0 

48 is Produced
48 - inserted in buffer
0 0 0 0 0 26 45 46 47 48 0 0 0 0 0 0 

49 is Produced
49 - inserted in buffer
0 0 0 0 0 26 45 46 47 48 49 0 0 0 0 0 

50 is Produced
50 - inserted in buffer
0 0 0 0 0 26 45 46 47 48 49 50 0 0 0 0 

51 is Produced
51 - inserted in buffer
0 0 0 0 0 26 45 46 47 48 49 50 51 0 0 0 

52 is Produced
52 - inserted in buffer
0 0 0 0 0 26 45 46 47 48 49 50 51 52 0 0 

53 is Produced
53 - inserted in buffer
0 0 0 0 0 26 45 46 47 48 49 50 51 52 53 0 

54 is Produced
54 - inserted in buffer
0 0 0 0 0 26 45 46 47 48 49 50 51 52 53 54 

55 is Produced
55 - inserted in buffer

55 0 0 0 0 26 45 46 47 48 49 50 51 52 53 54 

56 is Produced
56 - inserted in buffer
55 56 0 0 0 26 45 46 47 48 49 50 51 52 53 54 

57 is Produced
57 - inserted in buffer
55 56 57 0 0 26 45 46 47 48 49 50 51 52 53 54 

58 is Produced
58 - inserted in buffer
55 56 57 58 0 26 45 46 47 48 49 50 51 52 53 54 

26 - removed from buffer
55 56 57 58 0 0 45 46 47 48 49 50 51 52 53 54 

45 - removed from buffer
55 56 57 58 0 0 0 46 47 48 49 50 51 52 53 54 

46 - removed from buffer
55 56 57 58 0 0 0 0 47 48 49 50 51 52 53 54 

47 - removed from buffer
55 56 57 58 0 0 0 0 0 48 49 50 51 52 53 54 

48 - removed from buffer
55 56 57 58 0 0 0 0 0 0 49 50 51 52 53 54 

49 - removed from buffer
55 56 57 58 0 0 0 0 0 0 0 50 51 52 53 54 

50 - removed from buffer
55 56 57 58 0 0 0 0 0 0 0 0 51 52 53 54 

51 - removed from buffer
55 56 57 58 0 0 0 0 0 0 0 0 0 52 53 54 

52 - removed from buffer
55 56 57 58 0 0 0 0 0 0 0 0 0 0 53 54 

53 - removed from buffer
55 56 57 58 0 0 0 0 0 0 0 0 0 0 0 54 

54 - removed from buffer
55 56 57 58 0 0 0 0 0 0 0 0 0 0 0 0 

55 - removed from buffer
0 56 57 58 0 0 0 0 0 0 0 0 0 0 0 0 

56 - removed from buffer
0 0 57 58 0 0 0 0 0 0 0 0 0 0 0 0 

57 - removed from buffer
0 0 0 58 0 0 0 0 0 0 0 0 0 0 0 0 

58 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

27 is Produced
27 - inserted in buffer
0 0 0 0 27 0 0 0 0 0 0 0 0 0 0 0 

28 is Produced
28 - inserted in buffer
0 0 0 0 27 28 0 0 0 0 0 0 0 0 0 0 

29 is Produced
29 - inserted in buffer
0 0 0 0 27 28 29 0 0 0 0 0 0 0 0 0 

30 is Produced
30 - inserted in buffer
0 0 0 0 27 28 29 30 0 0 0 0 0 0 0 0 

31 is Produced
31 - inserted in buffer
0 0 0 0 27 28 29 30 31 0 0 0 0 0 0 0 

32 is Produced
32 - inserted in buffer
0 0 0 0 27 28 29 30 31 32 0 0 0 0 0 0 

59 is Produced
59 - inserted in buffer
0 0 0 0 27 28 29 30 31 32 59 0 0 0 0 0 

60 is Produced
60 - inserted in buffer
0 0 0 0 27 28 29 30 31 32 59 60 0 0 0 0 

61 is Produced
61 - inserted in buffer
0 0 0 0 27 28 29 30 31 32 59 60 61 0 0 0 

62 is Produced
62 - inserted in buffer
0 0 0 0 27 28 29 30 31 32 59 60 61 62 0 0 

63 is Produced
63 - inserted in buffer
0 0 0 0 27 28 29 30 31 32 59 60 61 62 63 0 

64 is Produced
64 - inserted in buffer
0 0 0 0 27 28 29 30 31 32 59 60 61 62 63 64 

27 - removed from buffer
0 0 0 0 0 28 29 30 31 32 59 60 61 62 63 64 

28 - removed from buffer
0 0 0 0 0 0 29 30 31 32 59 60 61 62 63 64 

29 - removed from buffer
0 0 0 0 0 0 0 30 31 32 59 60 61 62 63 64 

30 - removed from buffer
0 0 0 0 0 0 0 0 31 32 59 60 61 62 63 64 

31 - removed from buffer
0 0 0 0 0 0 0 0 0 32 59 60 61 62 63 64 

32 - removed from buffer
0 0 0 0 0 0 0 0 0 0 59 60 61 62 63 64 

59 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 60 61 62 63 64 

60 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 61 62 63 64 

61 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 62 63 64 

62 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 63 64 

63 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 64 

64 - removed from buffer
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 

kendrick@kendrick-Latitude-D620:~/Desktop$ 
*/
