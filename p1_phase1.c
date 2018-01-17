// included libraries
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

/* Parent process function
 * keeps track of time and waits on the child process
 * prints out the statistics */
private static void parentProcess(int startTime)
{
	// variables for calculating time
	int endTime;
	int timeTaken;

	wait(0); // waits for the child process to be finished
	endTime = gettimeofday(); // gets the time after the child has finished
	timeTaken = (endTime - startTime) * 100; // calculates the time in milliseconds

	/* print out statistics */
	printf("-- Statistics --");
	printf("\tElapsed Time: %d", timeTaken);
	printf("\tPage faults: "); // needs implementation
	printf("\tPage faults (relaimed): "); // needs implementation
}

/* Child process function
 * Exec to become the wanted function */
private static void childProcess(char * args)
{
	execvp(args[0], args);
}

int main(int argc, int argv[])
{
	int returnVal; // hold the value returned by fork
	String userInput; // hold the option chosen by the user
	int startTime; // hold the start time of the child process
	char * args[2];

	/* Initial startup title */
	printf("==== Mid-Day Commander, vO ====");

	while(true)
	{
		/* Print out all the options and take input */
		printf("What's Good Commander? What command would you like to run?");
		printf("\t0. whoami  : Prints out the result of the whoamicommand");
		printf("\t1. last    : Prints out the result of the last command");
		printf("\t2. ls      : Prints out the result of a listing on a user-specified path");
		printf("Option? (control C to exit): ");
		scanf("%s", userInput);

		/* attempting to handle user input error */
		if(userInput > 2 || userInput <0)
		{
			perror("The following error occured: ");
		}

		/* fork the parent process to create a child */
		returnVal = fork();
		startTime = gettimeofday();

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
					args[0] = {"whoami", 0};
					childProcess(args);
					break;
				case 1:
					args[0] = {"last", 0};
					childProcess(args);
					break;
				case 2:
					args[0] = {"ls", 0};
					childProcess(args);
					break;
				default:
					printf("Invalid input");
					break;
			}
		}
	}

}
