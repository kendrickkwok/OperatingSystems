//----------Kendrick Kwok----------//
//----------Coding Assignment #2----------//
//----------912351666----------//
//----------CSC 415----------//


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
//Keep prompting the user until the user wants to exit 
int flag = 1;
while (1) {

	//VARIABLE DECLARATIONS
	
	//Declare a process
	pid_t pid;
	char *progPath="";
	char pathHolder[100];
	//Tokens 
	char *keptTokens[256];
	int numberOfTokens = 0;
	int n = 250;
	//Get what user needs for the shell
	char sentence[250];
	char *token;

	//How to exit
	//char* exitMe = "exit\n";

	//PROGRAM

	printf("My Shell >> ");

	//First get the user to type what they want 
	fgets(sentence, n, stdin);
	size_t length = strlen(sentence);
	//Eliminates all /n characters in sentence 	
	if (sentence[length - 1] == '\n')
    	sentence[length - 1] = '\0';

	//If exit is typed in, goes though the tokens and token is then 0
	//--Indicates that the user wants to leave the program. 
	if (strcmp(sentence, "exit") == 0)
	{
	printf("Goodbye.\n");
	flag = 0;
	exit(0); 
	}

	//Allocates a certain memory in space, equal to the length of sentence
	char *copiedString = malloc (strlen(sentence) + 1);

	//Copies sentenced string to string of copiedString
	//Used because strtok destroys the string once it is tokenized
	strcpy(copiedString, sentence);
	//printf("%s", copiedString);

	//Tokenize the copied string into tokens
	//Used copied string and seperate the words with empty space
	token = strtok(copiedString, " ");


		//while there is something inside token space
		//tokenize into arguments so shell can recognize language
		while (token != NULL)
		{

		//copy tokens into keptTokens
		keptTokens[numberOfTokens] = token;

		//increment the keptTokens index
		//copy into token char*
		//printf("%s", token);
		token = strtok(NULL," ");
		numberOfTokens++;
		}

		//start the processes using fork, and execute the command 
		//set the last element of keptTokens to be 
		//--NULL for argument passing	
		keptTokens[numberOfTokens] = NULL;

		//eliminate any /n values that could be in pathHolder
		for (int i=0; i <strlen(pathHolder); i++)
		{
			if (pathHolder[i]=='\n')
			{
			pathHolder[i]='\0';
			}
		}

		//creates a child process
		pid = fork();

		if (pid < 0 )
		{
		printf("Fork Failed");
		return 1;
		}

			else if (pid == 0) {
			// child process
			//executing. Pass in arguments, with first parameter
			//-- to be the path, three arguments, and NULL
			execlp(keptTokens[0], keptTokens[0], keptTokens[1], 
					keptTokens[2], keptTokens[3], NULL);
			//exit the child process if the execution has failed			
			exit(0);
			}
				else { 
				// parent process
				wait(NULL);
				}
		}

return 0;
}

////**************************OUTPUT AND EXECUTION EXAMPLE ***************////
/*
kendrick@kendrick-Latitude-D620:~/Desktop$ gcc -std=c99 second.c
kendrick@kendrick-Latitude-D620:~/Desktop$ ./a.out
My Shell >> typing in an unrecognized command
My Shell >> ls 
2.24~  android-studio	  CSC 415 - Yue		       test2.txt~
2.26~  a.out		  first.c~		       test.c
2.29~  Aptana RadRails	  HW-Basic Data Types.pdf      third.c
3.26~  Assignment 3 #1 ~  Review on Midterm CSC 340 ~  third.c~
3.30~  before ~		  second.c		       Yosemite
4.4~   CSC 340 - Potter   second.c~
My Shell >> ls -l
total 304
-rw-rw-r--  1 kendrick kendrick   1353 Feb 24  2016 2.24~
-rw-rw-r--  1 kendrick kendrick    812 Feb 26  2016 2.26~
-rw-rw-r--  1 kendrick kendrick      0 Feb 29  2016 2.29~
-rw-rw-r--  1 kendrick kendrick    821 Mar 28 11:54 3.26~
-rw-rw-r--  1 kendrick kendrick      0 Mar 30 11:13 3.30~
-rw-rw-r--  1 kendrick kendrick    286 Apr  4 11:24 4.4~
drwxrwxr-x  7 kendrick kendrick   4096 May 26 15:33 android-studio
-rwxrwxr-x  1 kendrick kendrick   9188 Sep 15 23:48 a.out
drwxr-xr-x  8 kendrick kendrick   4096 Sep  9 17:28 Aptana RadRails
-rw-rw-r--  1 kendrick kendrick   5742 Mar 14  2016 Assignment 3 #1 ~
-rw-rw-r--  1 kendrick kendrick  12292 Mar 31 00:01 before ~
drwxrwxr-x 35 kendrick kendrick   4096 Jun  7 09:28 CSC 340 - Potter
drwxrwxr-x  2 kendrick kendrick   4096 Sep  5 01:11 CSC 415 - Yue
-rw-rw-r--  1 kendrick kendrick    712 Sep  5 01:09 first.c~
-rw-rw-r--  1 kendrick kendrick 204331 Sep  5 01:11 HW-Basic Data Types.pdf
-rw-rw-r--  1 kendrick kendrick   2490 Mar 14  2016 Review on Midterm CSC 340 ~
-rw-rw-r--  1 kendrick kendrick   2790 Sep 15 23:47 second.c
-rw-rw-r--  1 kendrick kendrick   2822 Sep 15 23:47 second.c~
-rw-rw-r--  1 kendrick kendrick      7 Feb 25  2016 test2.txt~
-rw-rw-r--  1 kendrick kendrick    353 Sep 15 00:31 test.c
-rw-rw-r--  1 kendrick kendrick    337 Sep 15 22:56 third.c
-rw-rw-r--  1 kendrick kendrick    327 Sep 15 22:55 third.c~
drwxrwxr-x  2 kendrick kendrick   4096 Jun 20 15:18 Yosemite
My Shell >> /bin/echo hello this is   
hello this is
My Shell >> exit
Goodbye.
kendrick@kendrick-Latitude-D620:~/Desktop$ ^C
*/
////*********************************************************************////
