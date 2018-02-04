/* test2.c
 * Andrew Morrison
 * Peter Christakos
 */

#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/kernel.h>
//334
#define __NR_cs3013_syscall2 378

/* Ancestry struct */
struct ancestry
{
	pid_t children[100];
	pid_t siblings[100];
	pid_t ancestors[10];
};

void procAncestry(int *pid1, int *pid2, int *pid3, int *pid4)
{
	// returns 0 to child
	int rc1, rc2, rc3, rc4;
	if((rc1 = fork())<0)
	{
		perror("~~~FATAL~~~\nFork Failed: \n");
		exit(1);
	}
	else if(rc1==0) // child1
	{
		*pid1 = getpid();
		printf("%d\n", *pid1);
		sleep(5);
		exit(0);
	}
	else //parent
	{
		if((rc2 = fork()) < 0)
		{
			perror("~~~FATAL~~~\nFork Failed: \n");
			exit(1);
		}
		else if(rc2==0) // child2
		{
			*pid2 = getpid();
			printf("%d\n", *pid2);
			sleep(5);
			exit(0);
		}
		else
		{
			if((rc3 = fork()) < 0)
			{
				perror("~~~FATAL~~~\nFork Failed: \n");
				exit(1);
			}
			else if(rc3 == 0) // child 3
			{	
				*pid3 = getpid();
				printf("%d\n", *pid3);
				sleep(5);
				exit(0);
			}
			else // parent 3
			{
				if ((rc4 = fork()) < 0)
				{
					perror("~~~FATAL~~~\nFork Failed: \n");
					exit(1);
				}
				else if (rc4 == 0) // child 4
				{
					*pid4 = getpid();
					printf("%d\n", *pid4);
					sleep(5);
					exit(0);
				}
				else // parent 4
				{
					sleep(5);
					exit(5);
				}
			}
		}
	}
}
		

int main()
{

	int pid1, pid2, pid3, pid4;
	/* Create our family tree */
	struct ancestry tree;// = malloc(sizeof (tree));
	procAncestry(&pid1, &pid2, &pid3, &pid4);
	pid_t pidArray[5] = {pid1, pid2, pid3, pid4};

	for(int i = 0; i<4; i++)
	{
		/* call the system call */
		printf("hello\n");
		printf("Hello %d\n", pidArray[i]);
		long ret = (long)syscall(__NR_cs3013_syscall2, pidArray[i], &tree);
		if(ret)
		{
			int j = 0;
			printf("\t~~~~~~~~KERNEL INTERCEPTOR TEST %d PROGRAM~~~~~~~~\n", (i+1));
			printf("Target pid: %d\n", pidArray[i]);
			printf("Test #: %d\n", (i+1));

			printf("\t~~~PRINTING CHILDREN - Test %d~~~\n", (i+1));
			for(j = 0; j<100; j++)
			{
				printf("[%i] Child PID: %d\n", j+1, tree.children[j]);
			}

			printf("\t~~~PRINTING SIBLINGS - Test %d~~~\n", (i+1));
			for(j = 0; j<100; j++)
			{
				printf("[%i] Sibling PID: %d\n", j+1, tree.siblings[j]);
			}

			printf("\t~~~PRINTING ANCESTORS - Test %d~~~\n", (i+1));
			for(j = 0; j<10; j++)
			{
				printf("[%i] Ancestor PID: %d\n", j+1, tree.ancestors[j]);
			}

		}
		else // failure
		{
			perror("~~~FATAL~~~\nSyscall failed: \n");
		}
	}
	return 0;
}
