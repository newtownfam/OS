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
	char *args[32];
	int numargs;
	int num;
	int bg;
	char *name;
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
	 * step 3. change waiting and implement the data structure*/

	/* variables for calculating time */
	long endTime, startTime, elapsedTime; // time variables
	int pagefaults; // pagefaults
	int pagefaults_r; // reclaimed pagefaults FIX THIS

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
int main(int argc, char ** argv)
{
	struct processes bgProcesses[200]; // holds background processes

	for(int j=0; j<200; j++)
	{
		bgProcesses[j].pid = 0;
		bgProcesses[j].num = 0;
		bgProcesses[j].bgPending = -1;
	}

	struct info commands[400]; // holds new commands
	struct info currentCommand;
        struct info emptyCommand;
	for(int i = 0; i < 32; i++)
	{
		emptyCommand.args[i] = NULL;
	}
	//int exit = 0; // hold the option chosen by the user
	int k = 3; // integer to keep track of user-added commands
	//FILE *file; // create the ability to read a file
	//char c; // character used to read input file
	int bgRunning = 0;

	
	/* Initial startup title */
	printf("\n ==== Mid-Day Commander, v1 ====\n");

	/* loop until exit is called*/
	while(1)
	{
		fflush(stdout);
		currentCommand = emptyCommand;
		char thing[100];
		char *buffer = NULL;
		int returnVal=0; // hold the value returned by fork
		char *args[100]; // holds the arguments
		//char argString[1024]; // holds the total argument string
		//argString[1023] = 0x0;
		char path[101]; // holds path for 2.
		char * dir=NULL; // holds directory name for c.
		char userInput[100]; // holds the char of user input
		int current = -1;
		char split[100];

		/* Print out all the options and take input */
		printf("\nWhat's popping Commander? What command would you like to run?\n");
		printf("\t0. whoami  : Prints out the result of the whoami command\n");
		printf("\t1. last    : Prints out the result of the last command\n");
		printf("\t2. ls      : Prints out the result of a listing on a user-specified path\n");

		/* Prints out all user-added commands */
		for(int n=3; n<k; n++)
		{
			printf("\t%d. %s\t: user added command\n", commands[n].num, commands[n].name);
		}

		/* prints out a, c, e, and p commands */
		printf("\ta. add command : adds a new command to the menu\n");
		printf("\tc. change directory : Changes process working directory\n"); // done: needs testing
		printf("\te. exit : exit midday Commander\n"); // working: exits terminal
		printf("\tp. pwd : Prints working directory\n");
		//printf("\tr. prints running background processes\n");
		printf("Option? (control C to exit): ");

		/* take input */
		if(fgets(userInput, 100, stdin)==NULL)
		{
			printf("End of file reached\n");
			break;
		}
		userInput[strlen(userInput)-1] = '\0';
		printf("User input: %s\n", userInput);
		
		if(userInput[0] == 'r')
		{
			printf("Invalid Argument\n");
			continue;
		}
		if(userInput[0] == 'e')
		{
			printf("Peace out, Commander\n");
			return 0;
		}
		/* Get arguments if necessary */
		if (userInput[0] == '2')
		{
			int i = 0;

			/* User input */
			printf("Arguments (type N for none): ");

			if(fgets(thing, 100, stdin)==NULL)
			{
				printf("End of file reached\n");
				break;
			}
			thing[strlen(thing)-1] = '\0';
			printf("Args: %s\n", thing);
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
		char option[100]; // holds the option 
		//int u = atoi(userInput);

		/* Fill option, args[0], etc. and perform commands */
		switch(userInput[0]) 
		{
			case '0':
				strncpy(option, "whoami", 100);
				args[0] = option;
				printf("--Who am I?--\n");
				break;
			case '1':
				strncpy(option, "last", 100);
				args[0] = option;
				break;
			case '2':
				strncpy(option, "ls", 100);
				printf("Specify Path: \n");
				if(fgets(path, 100, stdin) == NULL)
				{
					printf("End of file reached\n");
					break;
				}
				path[strlen(path)-1] = '\0';
				printf("Your path was: %s\n", path);
				args[0] = option; // store "ls"
				for(int i = 0;;i++){
				if (args[i] == NULL){
					args[i] = path; // store path name
					break;
				}
				}
				break;
			case 'a':
				printf("Enter function name for %d: \n", k);

				/* Save user-added command parameters into commands */
				if(fgets(split, 100, stdin) == NULL)
				{
					printf("End of file reached\n");
					break;
				}
				split[strlen(split)-1] = '\0';
				if (split[strlen(split)-1] == '&'){
					commands[k].bg = 0;
					split[strlen(split)-1] = '\0';
				}
				else
					commands[k].bg = -1;
				
				char* token = strtok(split, " ");
				commands[k].name = strdup(token);
				commands[k].args[0] = strdup(token);
				commands[k].numargs = 1;
				for(token = strtok(NULL, " "); token!=NULL; token = strtok(NULL, " ")){
				    commands[k].args[commands[k].numargs] = strdup(token);
				    commands[k].numargs++;
				}
				//strcpy(commands[k].name, thing);
				printf("Command: %s\n", split); 
				commands[k].num = k;// update ammount of user-added commands
				k++;
				break;
			case 'c':
				printf("Directory?: ");
				if(fgets(option, 100, stdin) == NULL)
				{
					printf("End of file reached\n");
					break;
				}
				printf("%s\n", option);
				option[strlen(option)-2] = '\0';
				chdir(option);
				printf("ADSFADS");
				break;
			case 'p':
				strncpy(option, "p", 100);
				args[0] = "pwd";
				break;
			case 'r':
				printf("\n~~~~~~Background boyes~~~~~~~\n");
				for(int m = 0; m<200; m++)
				{
					if(bgProcesses[m].num!=0)
					{
						printf("BG Process #: %d, BG PID: %d\n", bgProcesses[m].num, bgProcesses[m].pid);
					}
				}
				break;
			default: // default will handle user-added commands
				for(int n=3; n<k; n++)
				{	
					/* find the user-added command that matched userInput */
					if(atoi(&(userInput[0])) == commands[n].num)
					{
						/* run the necessary command */
						args[0] = commands[n].name;
						currentCommand = commands[n];
						current = currentCommand.bg;
						printf("\ncommands: %s\n", commands[n].name);
						break;	
					}
					
				}
			}

		fflush(stdout);
			if(userInput[0] != 'a' && userInput[0] != 'e' &&userInput[0] !='r' && userInput[0] != 'c')
			{
				returnVal = fork();
				if(returnVal > 0) // parent
				{
					if(current != 0) // not background
					{
						parentProcess(bgProcesses, current, bgRunning, returnVal);
					}
				}
				else if(returnVal == 0 && current == 0) // child and background
				{
					int spoon = fork();
					if(spoon == 0) 
					{
						execvp(args[0], currentCommand.args);
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
					currentCommand.args[0] = strdup(args[0]);
					for(int i = 0; i < 5; i++)
					{
						//printf("%s, ", currentCommand.args[i]);
					}
					printf("\n");
					execvp(args[0], currentCommand.args);
					perror("execvp\n");
					exit(1);
				}
				else
				{
					printf ("Error\n");
				}
				}
			
		}
		return 0;
	}	
