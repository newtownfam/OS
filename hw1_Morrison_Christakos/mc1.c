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
#include <assert.h>

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
	int rc = execvp(option, args); // system function to execute the three commands
	assert(rc==0);
}
/* Arguments function
 * Gets user inputted arguments */
void getArgs()
{
	NULL;
}

int main(int argc, int argv[])
{
	int returnVal; // hold the value returned by fork
	char userInput; // hold the option chosen by the user
	char *args[10]; // holds the arguments
	//char argString[1024]; // holds the total argument string
	//argString[1023] = 0x0;
	char buff; // buff to eat up the enter key
	//args[1] = 0;
	char path[20];
	char dir[1024];

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
			// child process gets a return value equal to zero

			/* Get arguments if necessary */
			if(userInput != 'a' && userInput != 'c' && userInput != 'e' && userInput != 'p')
			{

				char thing[100];
				char * buff;
				int i = 0;

				if(userInput == '2')
				{
					i++;
				}

				/* User input */
				printf("Arguments (type N for none): \n");
				scanf("%s", thing);
				printf("\n");
				if(strcmp(thing, "N") != 0)
				{
					/* Store the arguments in args[i] */
					buff = strtok(thing, " ");
					while(buff != NULL)
					{	
						i++;
						args[i] = buff;
						buff = strtok(NULL, " ");
					}
				}
				args[i+1] = NULL;
			}
			char * option; // holds the option 
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
					printf("Specify Path: \n");
					scanf("%s", path);
					printf("Your path was: %s\n", path);
					args[0] = option;
					args[1] = path;
					childProcess(option, args);
					break;
				case 'a':
					printf("exiting");
					break;
				case 'c':
					printf("Directory?: \n");
					scanf("%s", option);
					chdir(option);
					break;
				case 'e':
					printf("exiting");
					break;
				case 'p':
					if(getcwd(dir, sizeof(dir)) != NULL)
					{
						printf("Current directory: %s\n", dir);
					}
					else 
					{
						perror("Error using getcwd()");
					}
					break;
;				default:
					printf("Invalid input\n");
					break;
				}

			}
		}
		return 0;
	}	

