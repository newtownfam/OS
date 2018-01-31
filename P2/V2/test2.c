/* test2.c
 * Andrew Morrison
 * Peter Christakos
 */

#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/kernel.h>

#define __NR_cs3013_syscall2 378

/* Ancestry struct */
struct ancestry
{
	pid_t children[100];
	pid_t siblings[100];
	pid_t ancestors[10];
}

void procAncestry()
{
	int pid1, pid2, pid3, pid4;

	if((pid1 = fork())>=0)
	{
		if(pid1==0) // child1
		{
			if((pid2 = fork())>=0)
			{
				if(pid2==0) // child2
				{
					sleep(5);
					exit(0);
				}
				else // parent2
				{
					if((pid3 = fork())>=0)
					{
						if(fork == 0) // child 3
						{
							sleep(5);
							exit(0);
						}
						else // parent 3
						{
							sleep(5);
							exit(0);
						}
					}
					else // failure
					{
						perror("~~~FATAL~~~\nFork Failed: \n");
						exit(1);
					}
				}
			}
			else // failure
			{
				perror("~~~FATAL~~~\nFork Failed: \n");
				return 0;
			}
		}
		else // parent1
		{
			if((pid4 = fork())>=0)
			{
				if(pid4==0) // child 4
				{
					sleep(5);
					exit(0);
				}
				else // parent 4
				{
					// continue
				}
			}
			else // failure
			{
				perror("~~~FATAL~~~\nFork Failed: \n");
				exit(1);
			}
		}
	}
	else // failure
	{
		perror("~~~FATAL~~~\nFork Failed: \n");
		exit(1);
	}
}

int main()
{
	/* Create our family tree */
	struct ancestry * tree = malloc(sizeof(struct ancestry));
	procAncestry();
	pid_t pidArray[5] = {pid1, pid2, pid3, pid4};

	for(int i = 0; i<4; i++)
	{
		/* call the system call */
		if(!(Long ret = (long)syscall(__NR_cs3013_syscall2, pidArray[i], &tree)))
		{
			int j = 0;
			printf("\t~~~~~~~~KERNEL INTERCEPTOR TEST PROGRAM~~~~~~~~\n");
			printf("Target pid: %d\n\n", pid3);
			printf("Test #: %d\n", (i+1));

			printf("\t~~PRINTING CHILDREN~~\n");
			for(j = 0; j<99; j++)
			{
				printf("[%i] PID: %d\n", (i+1), tree.children[j]);
			}

			printf("\t~~PRINTING SIBLINGS~~\n");
			for(j = 0; j<99; j++)
			{
				printf("[%i] PID: %d\n", (i+1), tree.siblings[j]);
			}

			printf("\t~~PRINTING ANCESTORS~~\n");
			for(j = 0; j<99; j++)
			{
				printf("[%i] PID: %d\n", (i+1), tree.ancestors[j]);
			}

		}
		else // failure
		{
			perror("~~~FATAL~~~\nSyscall failed: \n");
		}
	}
	return 0;
}
