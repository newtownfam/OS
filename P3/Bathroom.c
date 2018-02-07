/* Bathroom.c
 * Andrew Morrison
 * Peter Christakos */

#include "Bathroom.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <sys/types.h>

/* enter
 * g is the gender of the thread
 * compare g with global bathroom gender
 * if brGlobal is vacant, then enter, set flag, increment counters, return 1
 * if g == brGlobal, then enter, increment counters, return 1
 * if g != brGlobal, return 0
 * if gender is invalid, return -1
 */



int enter(struct br* brGlobal, int g) // needs lockageeeeee
{
	if(brGlobal->gender == -1) // check if the bathroom is vacant, if so enter
	{
		brGlobal->gender = g; // set gender flag
		assert(brGlobal->mCount == 0 && brGlobal->fCount == 0);
		brGlobal->totalUsages++;
		switch(g)
		{
			case 0:
				brGlobal->fCount++; // increment female count
				break;
			case 1:
				brGlobal->mCount++; // increment male count
				break;
			default:
				return -1; // invalid gender
				break;
		}
		return 1; // success
	}
	else if(brGlobal->gender == g) // correct gender so enter
	{
		brGlobal->totalUsages++;
		switch(g)
		{
			case 0:
				assert(brGlobal->mCount == 0);
				brGlobal->fCount++; // increment female count
				break;
			case 1:
				assert(brGlobal->fCount == 0);
				brGlobal->mCount++; // increment male count
				break;
			default:
				return -1; // invalid gender
				break;
		}
		return 1; // success
	}
	else // would be inappropriate to enter at this time, begin waiting
	{
		return 0; // failure
	}
}

/* leave
 * if global bathroom is flagged as female then decrement the female counter 
 *  and check if bathroom is vacant, if so then set the flag to vacant
 * same idea for male
 */
void leave(struct br* brGlobal) // needs lockaging
{
	if(brGlobal->gender == 0) // if its a female
	{
		assert(brGlobal->mCount == 0);
		brGlobal->fCount--; // decrement females
		if(brGlobal->fCount == 0) // if last female to leave
		{
			brGlobal->gender = -1; // set flag to vacant
		}
	}
	if(brGlobal->gender == 1) // if its a male
	{
		assert(brGlobal->fCount == 0);
		brGlobal->mCount--; // decrement males
		if(brGlobal->mCount == 0) // if last male to leave
		{
			brGlobal->gender = -1; // set flag to vacant
		}
	}
}

/* Initializes the threads
 * Initialize the bathroom object
 */
void initialize(struct br* brGlobal)
{
  brGlobal = (struct br *)malloc(sizeof(struct br));
	brGlobal->gender = -1;
	brGlobal->mCount = 0;
	brGlobal->fCount = 0;
	brGlobal->totalUsages = 0;
	brGlobal->vacantTime = 0;
	brGlobal->occupiedTime = 0;
  
}

/* Prints out all statistics and exits
 */
void finalize(struct br* brGlobal)
{
	printf("\nTotal Usages: %d\nVacant Time: %l\nOccupied Time: %l\n", brGlobal->totalUsages, brGlobal->vacantTime, brGlobal->occupiedTime);
}
/* Prints out statistics for each individiaul thread before it exits
 * Will print:
 *	its own thread number
 *	its gender and number of loops
 *	the min, average, and max time spent waiting in the queue
 */
void printStats(pthread_mutex_t printLock, int gender, int threadNum, int lCount, long minTime, long aveTime, long maxTime)
{
	printf("~~~THREAD [%d] STATISTICS~~~\n", threadNum);
  printf("Gender: %");
}
