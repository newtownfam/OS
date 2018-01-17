// included libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

struct rusage usage; // struct created in order to use getrusage function

/* Parent process function
 * keeps track of time and waits on the child process
 * prints out the statistics */
void parentProcess(long startTime)
{
	// variables for calculating time
	long endTime;
	double timeTaken;
	int pagefaults; // pagefaults
	int pagefaults_r; // reclaimed pagefaults

	wait(0); // waits for the child process to be finished
	endTime = clock(); // gets the time after the child has finished
	//printf("Start time: %ld, End: %ld\n", startTime, endTime);
	timeTaken = ((double)(endTime - startTime)/CLOCKS_PER_SEC)*1000; // calculates the time in milliseconds
	
	/* use getrusage function for page faults (not sure if working properly) */
	getrusage(RUSAGE_CHILDREN, &usage); //RTFMP
	pagefaults_r = usage.ru_minflt; // getrusage function to show reclaimed page faults
	pagefaults = usage.ru_majflt; //shows non-reclaimed page faults

	/* print out statistics */
	printf("\n-- Statistics --\n");
	printf("\tElapsed Time: %f milliseconds\n", timeTaken);
	printf("\tPage faults: %d\n", pagefaults); // needs implementation
	printf("\tPage faults (relaimed): %d\n\n", pagefaults_r); // needs implementation
}

/* Child process function
 * Exec to become the wanted function */
void childProcess(char * option, char ** args)
{
	execvp(option, args); // system fcuntion to execute the three commands
}

int main(int argc, int argv[])
{
	int returnVal; // hold the value returned by fork
	int userInput; // hold the option chosen by the user
	long startTime; // hold the start time of the child process
	char * args[2];
	char * option;
	args[1] = NULL;

	/* Initial startup title */
	printf(" ==== Mid-Day Commander, vO ====\n");

	while(!0)
	{
		/* Print out all the options and take input */
		printf("What's popping Commander? What command would you like to run?\n");
		printf("\t0. whoami  : Prints out the result of the whoami command\n");
		printf("\t1. last    : Prints out the result of the last command\n");
		printf("\t2. ls      : Prints out the result of a listing on a user-specified path\n");
		printf("Option? (control C to exit): ");
		scanf("%d", &userInput);
		printf("\n");

		/* fork the parent process to create a child */
		returnVal = fork();
		time_t seconds;
		startTime = clock();

		/* Run the chlild or parent function depending on the return value */
		if(returnVal != 0)
		{
			// parent process gets a return value equal to the child PID
			parentProcess(startTime);
		}
		else
		{
			// child process gets a return value equal to 0
			switch(userInput) 
			{
				case 0:
					option = "whoami";
					args[0] = option;
					childProcess(option, args);
					break;
				case 1:
					option = "last";
					args[0] = option;
					childProcess(option, args);
					break;
				case 2:
					option = "ls";
					args[0] = option;
					childProcess(option, args);
					break;
				default:
					printf("Invalid input");
					break;
			}
		}
	}

}

