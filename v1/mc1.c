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
	long endTime, startTime, elapsedTime; // time variables
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
	getrusage(RUSAGE_CHILDREN, &usage); // RTFMP
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
	printf("~~~~~~~~~~%s~~~~~~~~~~\n", option);
	int rc = execvp(option, args); // system function to execute the three commands
	assert(rc==0);
}
/* Arguments function
 * NOT IN USE
 * Gets user inputted arguments */
/*char ** getArgs()
{
	char thing[100];
	char ** args;
	char * buff;
	char * guy;
	int i = 0;

	// User Input
	printf("Arguments: ");
	scanf("%s", thing);

	// Store the arguments in args[i]
	buff = strtok(thing, " ");
	while(buff != NULL)
	{	
		args[i] = buff;
		buff = strtok(NULL, " ");
		i++;
	}
	return args;
}*/

/* struct info holds the name and number of user commands
 */
struct info
{
	int num;
	char name[20];
};

/* main */
int main(int argc, char ** argv[])
{
	struct info commands[100]; // holds new commands
	int exit = 0; // hold the option chosen by the user
	int k = 3; // integer to keep track of user-added commands
	FILE *file; // create the ability to read a file
	char c; // character used to read input file

	
	/* Initial startup title */
	printf(" ==== Mid-Day Commander, vO ====\n");

	/* loop until exit is called*/
	while(exit != 1)
	{
		printf("exit: %i\n", exit);
		int returnVal; // hold the value returned by fork
		char *args[34]; // holds the arguments
		//char argString[1024]; // holds the total argument string
		//argString[1023] = 0x0;
		char buff; // buff to eat up the enter key
		char path[1024]; // holds path for 2.
		char dir[1024]; // holds directory name for c.
		char userInput; // holds the char of user input

		/* Print out all the options and take input */
		printf("What's popping Commander? What command would you like to run?\n");
		printf("\t0. whoami  : Prints out the result of the whoami command\n");
		printf("\t1. last    : Prints out the result of the last command\n");
		printf("\t2. ls      : Prints out the result of a listing on a user-specified path\n");

		/* Prints out all user-added commands */
		int n=3;
		while(n!=k)
		{
			printf("\t%d. user command: %s\n", commands[n].num, commands[n].name);
			n++;
		}

		/* prints out a, c, e, and p commands */
		printf("\ta. add command : adds a new command to the menu\n");
		printf("\tc. change directory : Changes process working directory\n"); // done: needs testing
		printf("\te. exit : exit midday Commander\n"); // working: exits terminal
		printf("\tp. pwd : Prints working directory\n");
		printf("Option? (control C to exit): ");

		/*For taking an input file */
		if (argv[1])
		{
			file = fopen(*argv[0], "r");
			if (file)
			{
				while(c != EOF)
				{
					putchar(c);
					c = fgetc(file);
					exit = 1;				
				}
			}
		}
		/* take input */
		scanf("%c", &userInput);
		scanf("%c", &buff);
		printf("\n");
		
		if(userInput == 'e')
			{
				printf("Peace out, Commander\n");
				return 0;
			}
		
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

				char thing[150];
				char * buffer;
				int i = 0;

				if(userInput == '2')
				{
					i++;
				}

				/* User input */
				printf("Arguments (type N for none): ");
				scanf("%s", thing);
				printf("\n");
				if(strcmp(thing, "N") != 0)
				{
					/* Store the arguments in args[i] */
					buffer = strtok(thing, " ");
					while(buffer != NULL)
					{	
						i++;
						args[i] = buffer;
						buffer = strtok(NULL, " ");
					}
				}
				args[i+1] = NULL;
			}
			char * option; // holds the option 
			char option2[1024]; // holds the option if scanf() must be used

			/* Fill option, args[0], etc. and perform commands */
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
					scanf("%s", path); // must specify a path for ls
					printf("Your path was: %s\n", path);
					args[0] = option; // store "ls"
					args[1] = path; // store path name
					childProcess(option, args);
					break;
				case 'a':
					printf("Enter function name: \n");

					/* Save user-added command parameters into commands */
					scanf("%s", commands[k].name);
					scanf("%c", &buff);
					commands[k].num = k;
					k++; // update ammount of user-added commands
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
				default: // default will handle user-added commands
					n = 3;
					while(n!=k)
					{	
						/* find the user-added command that matched userInput */
						if(atoi(&userInput) == commands[n].num)
						{
							/* run the necessary command */
							args[0] = commands[n].name;
							childProcess(commands[n].name, args);
							n++;
					}
					break;
				}

			}
		}
	}	
	return 0;
}