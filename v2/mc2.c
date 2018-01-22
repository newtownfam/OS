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

/* struct info holds the name and number of user commands
 */
struct info
{
	int num;
	char name[20];
};

struct processes
{
	int num;
	int pid;
	int bgPending;
	struct timeval start_time, end_time;
};

/* Parent process function
 * keeps track of time and waits on the child process
 * prints out the statistics */
int parentProcess(struct processes bgProcesses[], int bgPending, int bgRunning, pid_t rv)
{

	/* Implement background process capability
	 * step 1. create data structure to keep track of background processes running
	 * step 2. -r running processes list prints out the processes from step 1
	 * step 3. change waiting and implement the data structure

	/* variables for calculating time */
	long endTime, startTime, elapsedTime; // time variables
	int pagefaults; // pagefaults
	int pagefaults_r; // reclaimed pagefaults FIX THIS
	pid_t lastpid;


		/* find elapsed time */
		gettimeofday(&clockTime, NULL); // get time before child process starts
		startTime = (clockTime.tv_sec * 1000) + (clockTime.tv_usec / 1000); // convert to ms
		
		wait(NULL);
		//waitpid(rv, NULL, WNOHANG);

		gettimeofday(&clockTime, NULL); // get time after child process ends

		endTime = (clockTime.tv_sec * 1000) + (clockTime.tv_usec / 1000); // convert to ms
		elapsedTime = endTime - startTime; // record elapsed time

		/* use getrusage function for page faults (not sure if working properly) */
		getrusage(RUSAGE_CHILDREN, &usage); // RTFMP			
		pagefaults_r = usage.ru_minflt; // getrusage function to show reclaimed page faults
		pagefaults = usage.ru_majflt; //shows non-reclaimed page faults

		/* print out statistics */
		if(bgPending == 0)
		{
			printf("\n--  Background Task Complete --\n");
		}
		else
		{
			printf("\n-- Task Complete --\n");
		}
		printf("\tElapsed Time: %ld milliseconds\n", elapsedTime);
		printf("\tPage faults: %d\n", pagefaults); // needs implementation
		printf("\tPage faults (relaimed): %d\n\n", pagefaults_r); // needs implementation

		pagefaults = 0;

		return 0;

}

/* Child process function
 * Exec to become the wanted function */
void childProcess(char * option, char ** args)
{
	int rc = execvp(option, args); // system function to execute the three commands
	assert(rc==0);
}

/* main */
int main(int argc, char ** argv[])
{
	struct processes bgProcesses[200]; // holds background processes

	for(int j=0; j<200; j++)
	{
		bgProcesses[j].pid = 0;
		bgProcesses[j].num = 0;
		bgProcesses[j].bgPending = -1;
	}

	struct info commands[400]; // holds new commands
	//int exit = 0; // hold the option chosen by the user
	int k = 3; // integer to keep track of user-added commands
	//FILE *file; // create the ability to read a file
	//char c; // character used to read input file
	int bgRunning = 0;
	int bgTotal;
	int getLineCount;

	
	/* Initial startup title */
	printf("\n ==== Mid-Day Commander, v2 ====\n");

	/* loop until exit is called*/
	while(1)
	{
		char *thing = NULL;
		char *buffer = NULL;
		size_t buffSize = 80;
		int returnVal=0; // hold the value returned by fork
		char *args[100]; // holds the arguments
		//char argString[1024]; // holds the total argument string
		//argString[1023] = 0x0;
		char buff; // buff to eat up the enter key
		char path[1024]; // holds path for 2.
		char dir[1024]; // holds directory name for c.
		char userInput; // holds the char of user input
		int current = -1;
		int kill = 0;

		/* Print out all the options and take input */
		printf("\nWhat's popping Commander? What command would you like to run?\n");
		printf("\t0. whoami  : Prints out the result of the whoami command\n");
		printf("\t1. last    : Prints out the result of the last command\n");
		printf("\t2. ls      : Prints out the result of a listing on a user-specified path\n");

		/* Prints out all user-added commands */
		for(int n=3; n<k; n++)
		{
			printf("\t%d. user command: %s\n", commands[n].num, commands[n].name);
		}

		/* prints out a, c, e, and p commands */
		printf("\ta. add command : adds a new command to the menu\n");
		printf("\tc. change directory : Changes process working directory\n"); // done: needs testing
		printf("\te. exit : exit midday Commander\n"); // working: exits terminal
		printf("\tp. pwd : Prints working directory\n");
		printf("\tr. prints running background processes\n");
		printf("Option? (control C to exit): ");

		/* take input */
		getLineCount = getline(&thing, &buffSize, stdin);
		thing[getLineCount - 1] = '\0';
		userInput = thing[0];
		
		if(userInput == 'e')
			{
				printf("Peace out, Commander\n");
				return 0;
			}
		/* Get arguments if necessary */
		if(userInput != 'a' && userInput != 'c' && userInput != 'e' && userInput != 'p' && userInput != 'r')
		{

			
			int i = 0;

			if(userInput == '2')
			{
				i++;
			}

			/* User input */
			printf("Arguments (type N for none): ");
			getLineCount = getline(&thing, &buffSize, stdin);
			thing[getLineCount-1] = '\0';
			if(strcmp(thing, "N") != 0)
			{
				/* Store the arguments in args[i] */
				buffer = strtok(thing, " ");

				while(buffer != NULL)
				{	
					i++;
					if(strcmp(buffer, "&")==0)
					{
						bgProcesses[bgRunning].bgPending = 0;
						current = 0;
						bgProcesses[bgRunning].num = bgRunning;
						bgRunning++;
					}
					else
						args[i] = strdup(buffer);
					buffer = strtok(NULL, " ");

				}
			}

			args[i+1] = NULL;
		}
		//time_t seconds;


		/* Run the chlild or parent function depending on the return value */
		/*if(returnVal != 0)
		{
			if(current == 0)
			{
				bgProcesses[bgTotal].pid = returnVal;
			}

			// parent process gets a return value equal to the child PID
			kill = parentProcess(bgProcesses, current, bgRunning, returnVal);

			for(int i=0; i<200; i++)
			{
				if(bgProcesses[i].pid == kill)
				{
					bgProcesses[i].num = 0;
					bgProcesses[i].pid = 0;
				}
			}
		}*/
			// child process gets a return value equal to zero
			char * option; // holds the option 
			char option2[1024]; // holds the option if scanf() must be used
			//int u = atoi(userInput);

			/* Fill option, args[0], etc. and perform commands */
			switch(userInput) 
			{
				case '0':
					option = "whoami";
					args[0] = option;
					break;
				case '1':
					option = "last";
					args[0] = option;
					break;
				case '2':
					option = "ls";
					printf("Specify Path: \n");
					scanf("%s", path); // must specify a path for ls
					printf("Your path was: %s\n", path);
					args[0] = option; // store "ls"
					args[1] = path; // store path name
					break;
				case 'a':
					printf("Enter function name for %d: \n", k);

					/* Save user-added command parameters into commands */
					scanf("%s", commands[k].name);
					scanf("%c", &buff);
					commands[k].num = k;// update ammount of user-added commands
					k++;
					break;
				case 'c':
					printf("Directory?: \n");
					scanf("%s", option2); // scan for directory
					scanf("%c", &buff); // eat the enter key
					chdir(option2);
					break;
				case 'p':
					if(getcwd(dir, sizeof(dir)) != NULL)
					{
						printf("Current Directory: %s\n", dir);
					}
					break;
				case 'r':
					printf("\n~~~~~~Background boyes~~~~~~~\n");
					for(int m = 0; m<200; m++)
					{
						if(bgProcesses[m].num!=0 && bgProcesses[m].pid!=0)
						{
							printf("BG Process #: %d, BG PID: %d\n", bgProcesses[m].num, bgProcesses[m].pid);
						}
					}
					break;
				default: // default will handle user-added commands
					for(int n=3; n<k; n++)
					{	
						/* find the user-added command that matched userInput */
						if(atoi(&userInput) == commands[n].num)
						{
							/* run the necessary command */
							printf("here\n");
							printf("%s\n", commands[n].name);
							args[0] = commands[n].name;
							printf("\ncommands: %s\n", commands[n].name);
							break;	
						}
						
					}
				}

				if(userInput != 'a' && userInput != 'e' &&userInput !='r' && userInput != 'c' && userInput != 'p')
				{
				returnVal = fork();
				if(returnVal > 0) // parent
				{
					if(current != 0) // not background
					{
						parentProcess(bgProcesses, current, bgRunning, returnVal);
					}
					else // background
					{
						// do nothing
					}
				}
				else if(returnVal == 0 && current == 0) // child and background
				{
					int spoon = fork();
					if(spoon == 0) 
					{
						execvp(args[0], (char * const *)args);
					}
					else if(spoon > 0)
					{
						bgProcesses[bgRunning].pid = returnVal;
						parentProcess(bgProcesses, current, bgRunning, returnVal);
						bgProcesses[bgRunning].num = 0;
						bgRunning--;
						exit(0);
					}
					else
					{
						printf("Error\n");
					}
				}
				else if(returnVal == 0)
				{
					execvp(args[0], (char * const *)args);
				}
				else
				{
					printf ("Error\n");
				}
			}
			
		}
		return 0;
	}	