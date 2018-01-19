// included libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>

struct rusage usage; // struct created in order to use getrusage function
struct timeval clockTime; // struct to access gettimeofday

/* Parent process function
 * keeps track of time and waits on the child process
 * prints out the statistics */
void parentProcess()
{
	/* variables for calculating time */
	long endTime, startTime, elapsedTime;
	int pagefaults; // pagefaults
	int pagefaults_r; // reclaimed pagefaults

	
	/* find elapsed time */
	gettimeofday(&clockTime, NULL); // get time before child process starts
	startTime = (clockTime.tv_sec * 1000) + (clockTime.tv_usec / 1000); // convert to ms
	
	wait(NULL); // waits for the child process to be finished
	
	gettimeofday(&clockTime, NULL); // get time after child process ends
	endTime = (clockTime.tv_sec * 1000) + (clockTime.tv_usec / 1000); // convert to ms
	
	elapsedTime = endTime - startTime; // record elapsed time

	/* use getrusage function for page faults (not sure if working properly) */
	getrusage(RUSAGE_CHILDREN, &usage); //RTFMP
	pagefaults_r = usage.ru_minflt; // getrusage function to show reclaimed page faults
	pagefaults = usage.ru_majflt; //shows non-reclaimed page faults

	/* print out statistics */
	printf("\n-- Statistics --\n");
	printf("\tElapsed Time: %ld milliseconds\n", elapsedTime);
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
	char userInput; // hold the option chosen by the user
	char * args[2];
	char * option;
	char buff;
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
		printf("\ta. add command : adds a new command to the menu\n");
		printf("\tc. change directory :Changes process working directory");
		printf("\te. exit : exit midday Commander");
		printf("\tp. pwd : Prints working directory");
		printf("Option? (control C to exit): ");
		scanf("%c", &userInput);
		scanf("%c", &buff);
		printf("\n");

		/* fork the parent process to create a child */
		returnVal = fork();
		//time_t seconds;

		/* Run the chlild or parent function depending on the return value */
		if(returnVal != 0)
		{
			// parent process gets a return value equal to the child PID
			parentProcess();
		}
		else
		{
			// child process gets a return value equal to 0
			switch(userInput) 
			{
				case '0':
					option = "whoami";
					for(int i; i<strlen(option); i++)
					{
						printf("--- %s ---\n", option[i]);
					}
					args[0] = option;
					childProcess(option, args);
					break;
				case '1':
					option = "last";
					for(int i; i<strlen(option); i++)
					{
						printf("--- %s ---\n", option[i]);
					}
					args[0] = option;
					childProcess(option, args);
					break;
				case '2':
					option = "ls";
					for(int i; i<strlen(option); i++)
					{
						printf("--- %s ---\n", option[i]);
					}
					args[0] = option;
					childProcess(option, args);
					break;
				case 'a':
					option = 'a'
					for(int i; i<strlen(option); i++)
					{
						printf("--- %s ---\n", option[i]);
					}
					args[0] = option;
					childProcess(option, args);
				case 'c':
					option = 'c'
					for(int i; i<strlen(option); i++)
					{
						printf("--- %s ---\n", option[i]);
					{}
					args[0] = option;
					childProcess(option, args);
				case 'e':
					option = 'e'
					return 0;
				case 'p':
					option = 'p'
					for(int i; i<strlen(option); i++)
					{
						printf("--- %s ---\n", option[i]);
					}
					args[0] = option;
					childProcess(option, args);
				default:
					printf("Invalid input\n");
					break;
			}
		}
	}

}

