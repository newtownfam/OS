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
	int pagefaults_r; // reclaimed pagefaults FIX THIS

	
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
	int rc = execvp(option, args); // system fcuntion to execute the three commands
	assert(rc==0);
}

/* Arguments function
 * Gets user inputted arguments */
int getArgs(char * argString, char * args)
{
	/* User input */
	scanf("Arguments?: %s", argString);

	/* Check if string is too long */
	if(sizeof(argString) > 128)
	{
		printf("Error. String too long");
		return -1;
	}

	/* Store the arguments in args[i] */
	int i = 0;
	for(i=1; i<=32; i++)
	{
		args[i] = strtok(" ");
	}
	args[i+1] = NULL;
	return 0;
}

int main(int argc, int argv[])
{
	int returnVal; // hold the value returned by fork
	char userInput; // hold the option chosen by the user
	char * args[34]; // holds the arguments
	char * option[1024]; // holds the option
	char * argString[130]; // holds the total argument string
	char buff; // buff to eat up the enter key
	args[1] = NULL;

	/* Initial startup title */
	printf(" ==== Mid-Day Commander, vO ====\n");

	while(userInput != 'e')
	{
		/* Print out all the options and take input */
		printf("What's popping Commander? What command would you like to run?\n");
		printf("\t0. whoami  : Prints out the result of the whoami command\n");
		printf("\t1. last    : Prints out the result of the last command\n");
		printf("\t2. ls      : Prints out the result of a listing on a user-specified path\n");
		printf("\ta. add command : adds a new command to the menu\n");
		printf("\tc. change directory : Changes process working directory\n"); // done: needs testing
		printf("\te. exit : exit midday Commander\n"); // working: exits terminal
		printf("\tp. pwd : Prints working directory\n");
		printf("Option? (control C to exit): \n");
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
			// child process gets a return value equal to zero

			/* Get arguments if necessary */
			if(userInput != 'a' && userInput != 'c' && userInput != 'e' && userInput != "p")
			{
				returnVal = getArgs(argString, args);
				assert(returnVal == 0);
			}
			switch(userInput) 
			{
				case '0':
					option = "whoami";
					args[0] = option;
					childProcess(option, args);
					break;
				case '1':
					option = "last";
					args[0] = option;
					childProcess(option, args);
					break;
				case '2':
					option = "ls";
					args[0] = option;
					childProcess(option, args);
					break;
				case 'a':
					*option = 'a';
					args[0] = option;
					childProcess(option, args);
				case 'c':
					*option = 'c';
					scanf("Directory: %s", &option);
					chdir(option;
				case 'e':
					break;
				case 'p':
					*option = 'p';
					if(getcwd(option, sizeOf(option)) != NULL)
					{
						// print the cwd
					}
				default:
					printf("Invalid input\n");
					break;
				}

			}
		}
		return 0;
	}	

